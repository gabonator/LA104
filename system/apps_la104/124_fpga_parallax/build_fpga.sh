#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
BUILD_SERVER="http://192.168.10.160:5000"
PORT="/dev/cu.usbmodem1101"
BITSTREAM="parallax.fpg"

mkdir -p "$BUILD_DIR"

get_status() {
    curl -s "$BUILD_SERVER/status" | python3 -c "import sys,json; print(json.load(sys.stdin).get('status','unknown'))"
}

# === Step 1: Wait for server to be idle, then upload ===
echo "=== Step 1: Upload sources ==="
resp=$(curl -s -F "files=@$SCRIPT_DIR/source/app.v" -F "files=@$SCRIPT_DIR/source/app.asf" "$BUILD_SERVER/upload")
echo "  $resp"
count=$(echo "$resp" | python3 -c "import sys,json; print(json.load(sys.stdin).get('count',0))")
if [ "$count" != "2" ]; then
    echo "ERROR: Upload failed: $resp"
    exit 1
fi

# === Step 2: Build ===
echo "=== Step 2: Start build ==="
resp=$(curl -s -X POST "$BUILD_SERVER/build")
echo "  $resp"
state=$(echo "$resp" | python3 -c "import sys,json; print(json.load(sys.stdin).get('status',''))")
if [ "$state" != "started" ]; then
    echo "ERROR: Build not started: $resp"
    exit 1
fi

# === Step 3: Wait ===
echo "=== Step 3: Waiting for build ==="
while true; do
    resp=$(curl -s "$BUILD_SERVER/status")
    state=$(echo "$resp" | python3 -c "import sys,json; print(json.load(sys.stdin).get('status',''))")
    lines=$(echo "$resp" | python3 -c "import sys,json; print(json.load(sys.stdin).get('log_lines',0))")

    if [ "$state" = "success" ]; then
        echo ""
        echo "  Build succeeded ($lines log lines)"
        break
    elif [ "$state" = "failed" ] || [ "$state" = "error" ]; then
        echo ""
        echo "  BUILD FAILED!"
        echo "  --- Last 30 lines of build log ---"
        curl -s "$BUILD_SERVER/log/full" | tail -30
        echo "  ---------------------------------"
        exit 1
    fi

    printf "\r  Building... (%s lines)" "$lines"
    sleep 5
done

# === Step 4: Download ===
echo "=== Step 4: Download bitstream ==="
curl -s "$BUILD_SERVER/artifact" -o "$BUILD_DIR/$BITSTREAM"
md5sum=$(md5 -q "$BUILD_DIR/$BITSTREAM")
size=$(stat -f%z "$BUILD_DIR/$BITSTREAM")
echo "  app.bin: $size bytes, MD5: $md5sum"
echo "=== Done ==="
