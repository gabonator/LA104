"""
Remote FPGA Build Server
Runs on Windows 7 with Quartus/AGM toolchain installed.
Accepts Verilog source files, runs synthesis in a temp folder,
streams raw stdout bytes over WebSocket (preserving ANSI codes).

Requirements: pip install flask flask-sock
Usage: python build_server.py --tools-dir C:\path\to\tools [--port 5000]

The --tools-dir should contain quartus\bin and Supra\bin subdirectories.
"""

import os
import sys
import json
import shutil
import tempfile
import argparse
import threading
import subprocess
from datetime import datetime
from flask import Flask, request, jsonify, Response, send_file, send_from_directory
from flask_sock import Sock

app = Flask(__name__)
sock = Sock(app)

# ---------------------------------------------------------------------------
# Config (set from args)
# ---------------------------------------------------------------------------
TOOLS_DIR = None
UPLOAD_DIR = None
ARTIFACT_DIR = None
DEVICE = "AG1KLPQ48"
DEVICE_FAMILY = "1"

# ---------------------------------------------------------------------------
# State
# ---------------------------------------------------------------------------
class BuildState:
    IDLE = "idle"
    BUILDING = "building"
    SUCCESS = "success"
    FAILED = "failed"

state_lock = threading.Lock()
build_state = {
    "status": BuildState.IDLE,
    "started": None,
    "finished": None,
    "exit_code": None,
    "error": None,
}
# Raw byte log - stores all output for replay to late-joining clients
raw_log = bytearray()
raw_log_lock = threading.Lock()
ws_clients = set()
ws_lock = threading.Lock()

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def reset_state():
    global raw_log
    with state_lock:
        build_state["status"] = BuildState.IDLE
        build_state["started"] = None
        build_state["finished"] = None
        build_state["exit_code"] = None
        build_state["error"] = None
    with raw_log_lock:
        raw_log = bytearray()

def broadcast_raw(data):
    """Send raw bytes to all WS clients as binary frames."""
    with raw_log_lock:
        raw_log.extend(data)
    with ws_lock:
        dead = set()
        for ws in ws_clients:
            try:
                ws.send(data)
            except Exception:
                dead.add(ws)
        for d in dead:
            ws_clients.discard(d)

def broadcast_msg(text):
    """Send a server message as raw bytes (with newline)."""
    broadcast_raw((text + "\r\n").encode("utf-8"))

def broadcast_status():
    """Push current status to all WS clients as a JSON text frame."""
    with state_lock:
        s = dict(build_state)
    artifact_path = os.path.join(ARTIFACT_DIR, "app.bin")
    if os.path.isfile(artifact_path):
        s["artifact_size"] = os.path.getsize(artifact_path)
    msg = json.dumps({"type": "status", **s})
    with ws_lock:
        dead = set()
        for ws in ws_clients:
            try:
                ws.send(msg)
            except Exception:
                dead.add(ws)
        for d in dead:
            ws_clients.discard(d)

def run_cmd(args, cwd, env, label):
    """Run a command, streaming raw stdout bytes. Returns exit code."""
    broadcast_msg(f"\x1b[36m[{label}] > {' '.join(args)}\x1b[0m")
    try:
        creationflags = 0
        if sys.platform == "win32":
            creationflags = subprocess.CREATE_NEW_PROCESS_GROUP
            env = dict(env)
            env["TERM"] = "xterm-256color"
            env["ANSICON"] = "1"
        proc = subprocess.Popen(
            args, cwd=cwd, env=env,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            bufsize=0, creationflags=creationflags,
        )
        while True:
            chunk = proc.stdout.read(4096)
            if not chunk:
                break
            broadcast_raw(chunk)
        proc.stdout.close()
        proc.wait()
        broadcast_msg(f"\x1b[36m[{label}] exit code: {proc.returncode}\x1b[0m")
        return proc.returncode
    except FileNotFoundError as e:
        broadcast_msg(f"\x1b[31m[{label}] ERROR: {e}\x1b[0m")
        return -1

def run_build():
    """Full build pipeline in a temporary directory."""
    global raw_log
    with raw_log_lock:
        raw_log = bytearray()

    with state_lock:
        build_state["status"] = BuildState.BUILDING
        build_state["started"] = datetime.now().isoformat()
        build_state["finished"] = None
        build_state["exit_code"] = None
        build_state["error"] = None
    broadcast_status()

    env = os.environ.copy()
    quartus_bin = os.path.join(TOOLS_DIR, "quartus", "bin")
    supra_bin = os.path.join(TOOLS_DIR, "Supra", "bin")
    env["PATH"] = quartus_bin + ";" + supra_bin + ";" + env.get("PATH", "")

    af_exe = os.path.join(supra_bin, "af.exe")
    quartus_sh = os.path.join(quartus_bin, "quartus_sh.exe")

    for tool, path in [("af.exe", af_exe), ("quartus_sh", quartus_sh)]:
        if not os.path.isfile(path):
            broadcast_msg(f"\x1b[31m[server] ERROR: {tool} not found at {path}\x1b[0m")
            with state_lock:
                build_state["status"] = BuildState.FAILED
                build_state["finished"] = datetime.now().isoformat()
                build_state["error"] = f"{tool} not found"
            broadcast_status()
            return

    uploaded = os.listdir(UPLOAD_DIR) if os.path.isdir(UPLOAD_DIR) else []
    if not uploaded:
        broadcast_msg("\x1b[31m[server] ERROR: No source files uploaded\x1b[0m")
        with state_lock:
            build_state["status"] = BuildState.FAILED
            build_state["finished"] = datetime.now().isoformat()
            build_state["error"] = "No source files"
        broadcast_status()
        return

    tmp_base = tempfile.mkdtemp(prefix="fpga_build_")
    app_dir = os.path.join(tmp_base, "app")
    os.makedirs(app_dir)
    broadcast_msg(f"\x1b[35m[server] Build directory: {app_dir}\x1b[0m")
    broadcast_msg(f"\x1b[35m[server] Started at {datetime.now().strftime('%H:%M:%S')}\x1b[0m")

    try:
        # Step 1: af.exe setup
        rc = run_cmd([
            af_exe, "-B", "--setup", "--mode", "QUARTUS",
            "--design", "app", "--device", DEVICE,
            "-X", f"set DEVICE_FAMILY {DEVICE_FAMILY}"
        ], cwd=app_dir, env=env, label="af setup")
        if rc != 0:
            raise RuntimeError(f"af.exe setup failed (exit {rc})")

        # Step 2: Copy source files
        for fname in uploaded:
            src = os.path.join(UPLOAD_DIR, fname)
            dst = os.path.join(app_dir, fname)
            shutil.copy2(src, dst)
            broadcast_msg(f"\x1b[35m[server] Copied {fname}\x1b[0m")

        # Step 3: quartus_sh synthesis
        rc = run_cmd([
            quartus_sh, "-t", "af_quartus.tcl"
        ], cwd=app_dir, env=env, label="quartus_sh")
        if rc != 0:
            raise RuntimeError(f"quartus_sh failed (exit {rc})")

        # Step 4: af.exe batch (fitting)
        rc = run_cmd([
            af_exe, "-B", "--batch", "--mode", "QUARTUS",
            "-X", "set QUARTUS_SDC true",
            "-X", "set FITTING timing_more",
            "-X", "set FITTER full",
            "-X", "set EFFORT highest",
            "-X", "set HOLDX default",
            "-X", "set SKEW basic",
        ], cwd=app_dir, env=env, label="af batch")
        if rc != 0:
            raise RuntimeError(f"af.exe batch failed (exit {rc})")

        # Step 5: Copy artifact
        src_bin = os.path.join(app_dir, "app.bin")
        dst_bin = os.path.join(ARTIFACT_DIR, "app.bin")
        if os.path.isfile(src_bin):
            shutil.copy2(src_bin, dst_bin)
            size = os.path.getsize(dst_bin)
            broadcast_msg(f"\x1b[32m[server] Artifact ready: app.bin ({size} bytes)\x1b[0m")
        else:
            raise RuntimeError("app.bin not produced")

        # Step 6: Compare with reference if exists
        ref_bin = os.path.join(ARTIFACT_DIR, "app.ref")
        if os.path.isfile(ref_bin):
            run_cmd(["fc", "/b", dst_bin, ref_bin], cwd=ARTIFACT_DIR, env=env, label="compare")

        with state_lock:
            build_state["status"] = BuildState.SUCCESS
            build_state["exit_code"] = 0

    except RuntimeError as e:
        broadcast_msg(f"\x1b[31m[server] FAILED: {e}\x1b[0m")
        with state_lock:
            build_state["status"] = BuildState.FAILED
            build_state["exit_code"] = 1
            build_state["error"] = str(e)

    except Exception as e:
        broadcast_msg(f"\x1b[31m[server] Exception: {e}\x1b[0m")
        with state_lock:
            build_state["status"] = BuildState.FAILED
            build_state["exit_code"] = 1
            build_state["error"] = str(e)

    finally:
        try:
            shutil.rmtree(tmp_base)
            broadcast_msg(f"\x1b[35m[server] Cleaned up temp directory\x1b[0m")
        except Exception as e:
            broadcast_msg(f"\x1b[33m[server] Cleanup warning: {e}\x1b[0m")

        with state_lock:
            build_state["finished"] = datetime.now().isoformat()
            if build_state["started"]:
                dt = datetime.fromisoformat(build_state["finished"]) - datetime.fromisoformat(build_state["started"])
                elapsed = int(dt.total_seconds())
            else:
                elapsed = 0

        broadcast_msg(f"\x1b[35m[server] Build finished ({elapsed}s)\x1b[0m")
        broadcast_status()

# ---------------------------------------------------------------------------
# WebSocket endpoint
# ---------------------------------------------------------------------------
@sock.route("/ws")
def ws_handler(ws):
    with ws_lock:
        ws_clients.add(ws)
    try:
        # Replay raw log for late-joining clients
        with raw_log_lock:
            if raw_log:
                ws.send(bytes(raw_log))
        # Send current status
        with state_lock:
            s = dict(build_state)
        artifact_path = os.path.join(ARTIFACT_DIR, "app.bin")
        if os.path.isfile(artifact_path):
            s["artifact_size"] = os.path.getsize(artifact_path)
        ws.send(json.dumps({"type": "status", **s}))
        # Keep alive
        while True:
            data = ws.receive(timeout=60)
            if data is None:
                break
    except Exception:
        pass
    finally:
        with ws_lock:
            ws_clients.discard(ws)

# ---------------------------------------------------------------------------
# HTTP Routes
# ---------------------------------------------------------------------------
@app.route("/")
def index():
    return send_from_directory(os.path.dirname(os.path.abspath(__file__)), "index.html")

@app.route("/upload", methods=["POST"])
def upload():
    with state_lock:
        if build_state["status"] == BuildState.BUILDING:
            return jsonify({"error": "Build in progress"}), 409

    os.makedirs(UPLOAD_DIR, exist_ok=True)
    for f in os.listdir(UPLOAD_DIR):
        os.remove(os.path.join(UPLOAD_DIR, f))

    saved = []
    files = request.files.getlist("files") or request.files.getlist("file")
    if not files:
        return jsonify({"error": "No files provided"}), 400

    for f in files:
        if f.filename:
            safe_name = os.path.basename(f.filename)
            f.save(os.path.join(UPLOAD_DIR, safe_name))
            saved.append(safe_name)

    return jsonify({"uploaded": saved, "count": len(saved)})

@app.route("/build", methods=["POST"])
def build():
    with state_lock:
        if build_state["status"] == BuildState.BUILDING:
            return jsonify({"error": "Build already in progress"}), 409

    threading.Thread(target=run_build, daemon=True).start()
    return jsonify({"status": "started"})

@app.route("/status")
def status():
    with state_lock:
        s = dict(build_state)
    s["log_lines"] = len(raw_log)
    artifact_path = os.path.join(ARTIFACT_DIR, "app.bin")
    if os.path.isfile(artifact_path):
        s["artifact_size"] = os.path.getsize(artifact_path)
    return jsonify(s)

@app.route("/log/full")
def log_full():
    with raw_log_lock:
        data = bytes(raw_log)
    return Response(data, mimetype="application/octet-stream")

@app.route("/artifact")
def artifact():
    artifact_path = os.path.join(ARTIFACT_DIR, "app.bin")
    if not os.path.isfile(artifact_path):
        return jsonify({"error": "No artifact available"}), 404
    return send_file(artifact_path, as_attachment=True, download_name="app.bin")

@app.route("/reset", methods=["POST"])
def reset():
    with state_lock:
        if build_state["status"] == BuildState.BUILDING:
            return jsonify({"error": "Build in progress"}), 409
    reset_state()
    return jsonify({"status": "reset"})

@app.route("/files")
def list_files():
    if not os.path.isdir(UPLOAD_DIR):
        return jsonify({"files": []})
    files = []
    for f in os.listdir(UPLOAD_DIR):
        fp = os.path.join(UPLOAD_DIR, f)
        files.append({"name": f, "size": os.path.getsize(fp)})
    return jsonify({"files": files})

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Remote FPGA Build Server")
    parser.add_argument("--port", type=int, default=5000)
    parser.add_argument("--host", default="0.0.0.0")
    parser.add_argument("--tools-dir", required=True,
                        help="Directory containing quartus/bin and Supra/bin")
    parser.add_argument("--device", default=DEVICE)
    parser.add_argument("--device-family", default=DEVICE_FAMILY)
    args = parser.parse_args()

    TOOLS_DIR = os.path.abspath(args.tools_dir)
    DEVICE = args.device
    DEVICE_FAMILY = args.device_family

    server_dir = os.path.dirname(os.path.abspath(__file__))
    UPLOAD_DIR = os.path.join(server_dir, "_uploads")
    ARTIFACT_DIR = os.path.join(server_dir, "_artifacts")
    os.makedirs(UPLOAD_DIR, exist_ok=True)
    os.makedirs(ARTIFACT_DIR, exist_ok=True)

    for sub in ["quartus/bin", "Supra/bin"]:
        p = os.path.join(TOOLS_DIR, sub.replace("/", os.sep))
        if not os.path.isdir(p):
            print(f"WARNING: {p} not found")

    print(f"Build server: http://{args.host}:{args.port}")
    print(f"Tools: {TOOLS_DIR}")
    print(f"Device: {DEVICE} (family {DEVICE_FAMILY})")

    app.run(host=args.host, port=args.port)
