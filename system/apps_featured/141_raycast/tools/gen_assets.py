#!/usr/bin/env python3
# Generates procedural wall textures, sprites and weapon for the raycaster,
# quantizes them to one shared 255 colour palette (index 0 = transparent)
# and writes source/assets.h with column-major 8 bit images, opaque spans of
# sprite columns and RGB565 shade tables (distance fog).
import sys, random
import numpy as np
from PIL import Image, ImageDraw

random.seed(1993)
rng = np.random.default_rng(1993)
T = 64
TRANSP = (255, 0, 255)

def noise(h, w, amp, scale=1):
    n = rng.normal(0, amp, (h // scale + 1, w // scale + 1))
    n = np.kron(n, np.ones((scale, scale)))[:h, :w]
    return n

def clamp(a):
    return np.clip(a, 0, 255).astype(np.uint8)

def img_from(arr):
    return Image.fromarray(clamp(arr), "RGB")

# ---------------------------------------------------------------- walls
def tex_brick(base=(150, 60, 45), mortar=(95, 90, 85)):
    a = np.zeros((T, T, 3))
    a[:] = mortar
    a += noise(T, T, 8)[..., None]
    bh, bw = 16, 32
    for row in range(T // bh):
        off = (bw // 2) if row % 2 else 0
        for col in range(-1, T // bw + 1):
            x0 = col * bw + off + 1
            y0 = row * bh + 1
            shade = rng.uniform(0.8, 1.15)
            for y in range(y0, y0 + bh - 2):
                for x in range(x0, x0 + bw - 2):
                    if 0 <= x < T:
                        v = np.array(base) * shade
                        # bevel: light top/left, dark bottom
                        if y == y0: v = v * 1.25
                        if y == y0 + bh - 3: v = v * 0.7
                        a[y, x] = v
    a += noise(T, T, 10)[..., None]
    return a

def tex_stone(base=(120, 120, 125), moss=False):
    a = np.zeros((T, T, 3))
    a[:] = (60, 60, 64)
    blocks = [(0, 0, 24, 20), (24, 0, 64, 20), (0, 20, 40, 42), (40, 20, 64, 42),
              (0, 42, 18, 64), (18, 42, 50, 64), (50, 42, 64, 64)]
    for (x0, y0, x1, y1) in blocks:
        s = rng.uniform(0.75, 1.1)
        blk = np.array(base) * s + noise(y1 - y0, x1 - x0, 12, 2)[..., None]
        blk[1:3, :] *= 1.2
        blk[:, 1:3] *= 1.12
        blk[-3:-1, :] *= 0.7
        blk[:, -3:-1] *= 0.75
        a[y0 + 1:y1 - 1, x0 + 1:x1 - 1] = blk[1:-1, 1:-1]
    if moss:
        m = noise(T, T, 1, 4) + noise(T, T, 0.6, 2)
        grad = np.linspace(-0.6, 1.0, T)[:, None]
        mask = (m + grad) > 0.7
        a[mask] = a[mask] * 0.4 + np.array((40, 110, 35)) * 0.6
    return a

def tex_wood():
    a = np.zeros((T, T, 3))
    for p in range(4):
        x0 = p * 16
        tone = rng.uniform(0.85, 1.1)
        for x in range(x0, x0 + 16):
            grain = np.sin(np.arange(T) * 0.35 + x * 0.9 + p * 3) * 10 + noise(T, 1, 6)[:, 0]
            col = np.array((125, 80, 40)) * tone
            a[:, x] = col + grain[:, None]
        a[:, x0] *= 0.55
        a[:, x0 + 15] *= 0.75
        for ny in (6, 57):
            a[ny - 1:ny + 2, x0 + 7:x0 + 9] = (60, 55, 50)
    a[30:34, :] = (70, 45, 25)
    return a

def tex_metal(base=(95, 105, 125)):
    a = np.zeros((T, T, 3))
    a[:] = base
    a += noise(T, T, 5)[..., None]
    a += np.linspace(14, -14, T)[None, :, None]
    for y in (0, 31, 32, 63):
        a[y, :] *= 0.55
    for x in (0, 63):
        a[:, x] *= 0.55
    a[1, :] *= 1.3
    a[33, :] *= 1.3
    for (x, y) in [(4, 4), (59, 4), (4, 27), (59, 27), (4, 36), (59, 36), (4, 59), (59, 59)]:
        a[y - 1:y + 2, x - 1:x + 2] = (170, 175, 185)
        a[y + 1, x - 1:x + 2] = (50, 55, 65)
    return a

def tex_tech():
    a = np.zeros((T, T, 3))
    a[:] = (45, 50, 58)
    a += noise(T, T, 4)[..., None]
    a[6:58, 6:58] = (30, 34, 40)
    a[6, 6:58] = (80, 88, 100)
    a[6:58, 6] = (80, 88, 100)
    a[57, 6:58] = (20, 22, 26)
    a[6:58, 57] = (20, 22, 26)
    for i, y in enumerate(range(12, 52, 8)):
        a[y:y + 3, 12:40] = (20, 80 + i * 20, 40)
        a[y:y + 3, 12:12 + 4 + i * 5] = (60, 230, 90)
    for i, y in enumerate((14, 30, 46)):
        c = [(230, 40, 30), (240, 200, 40), (40, 200, 240)][i]
        a[y:y + 5, 46:51] = c
        a[y, 46:51] = np.array(c) * 1.2
    return a

def tex_door():
    a = tex_metal((110, 100, 70))
    for y in range(8, 56, 6):
        a[y:y + 3, 10:54] *= 0.75
    a[28:36, 46:52] = (200, 190, 120)
    a[30:34, 47:51] = (90, 80, 40)
    # hazard stripes at the bottom
    for y in range(56, 63):
        for x in range(1, 63):
            a[y, x] = (220, 180, 30) if ((x + y) // 4) % 2 else (30, 30, 30)
    return a

def tex_exit():
    a = tex_metal((70, 80, 95))
    a[12:52, 14:50] = (25, 30, 35)
    # green panel with arrow
    a[16:30, 18:46] = (20, 120, 40)
    for i in range(10):
        a[18 + i // 2 + 2, 22 + i:42 - i] = (110, 255, 130) if i < 9 else a[18, 22]
    # switch lever
    a[34:48, 30:34] = (180, 180, 190)
    a[34:38, 26:38] = (230, 40, 30)
    return a

def tex_frame():
    a = tex_metal((85, 85, 95))
    a[:, 28:36] *= 0.7
    return a

walls = [
    ("brick", tex_brick()),
    ("stone", tex_stone()),
    ("wood", tex_wood()),
    ("metal", tex_metal()),
    ("tech", tex_tech()),
    ("moss", tex_stone((105, 110, 100), moss=True)),
    ("door", tex_door()),
    ("exit", tex_exit()),
    ("frame", tex_frame()),
]

# ---------------------------------------------------------------- sprites
def new_sprite(w=64, h=64):
    im = Image.new("RGB", (w, h), TRANSP)
    return im, ImageDraw.Draw(im)

def robot(frame):
    im, d = new_sprite()
    body = (120, 125, 135)
    dark = (60, 62, 70)
    light = (175, 180, 190)
    if frame == "dead":
        d.ellipse((10, 52, 54, 63), fill=dark)
        d.rectangle((14, 50, 38, 58), fill=body)
        d.ellipse((34, 46, 48, 58), fill=light)
        d.rectangle((40, 52, 44, 54), fill=(120, 20, 20))
        d.line((20, 49, 26, 44), fill=(255, 200, 60))
        d.line((24, 50, 30, 47), fill=(255, 120, 40))
        return im
    if frame == "die":
        d.rectangle((18, 36, 46, 56), fill=body, outline=dark)
        d.ellipse((22, 26, 42, 42), fill=light, outline=dark)
        d.rectangle((26, 32, 38, 36), fill=(90, 10, 10))
        d.rectangle((16, 56, 48, 63), fill=dark)
        d.ellipse((12, 18, 30, 34), fill=(255, 210, 60))
        d.ellipse((16, 22, 26, 30), fill=(255, 255, 200))
        return im
    lx = {"walk1": (-3, 3), "walk2": (3, -3)}.get(frame, (0, 0))
    # legs
    d.rectangle((22 + lx[0], 44, 29 + lx[0], 62), fill=dark)
    d.rectangle((35 + lx[1], 44, 42 + lx[1], 62), fill=dark)
    d.rectangle((20 + lx[0], 60, 30 + lx[0], 63), fill=(40, 40, 45))
    d.rectangle((34 + lx[1], 60, 44 + lx[1], 63), fill=(40, 40, 45))
    # torso
    d.polygon([(18, 22), (46, 22), (43, 46), (21, 46)], fill=body, outline=dark)
    d.rectangle((24, 28, 40, 38), fill=(85, 90, 100))
    d.rectangle((28, 31, 36, 34), fill=(220, 160, 30))
    # head
    d.ellipse((24, 6, 40, 22), fill=light, outline=dark)
    eye = (255, 40, 30) if frame != "pain" else (255, 255, 255)
    d.rectangle((27, 12, 37, 15), fill=(40, 10, 10))
    d.rectangle((29, 12, 35, 15), fill=eye)
    # arms + gun
    d.rectangle((12, 24, 18, 42), fill=dark)
    if frame == "shoot":
        d.rectangle((44, 26, 58, 31), fill=dark)
        d.rectangle((46, 24, 62, 28), fill=(40, 40, 45))
        d.ellipse((50, 14, 63, 30), fill=(255, 210, 60))
        d.ellipse((54, 18, 61, 26), fill=(255, 255, 210))
    else:
        d.rectangle((46, 24, 52, 40), fill=dark)
        d.rectangle((44, 38, 56, 44), fill=(40, 40, 45))
    im = np.array(im).astype(float)
    if frame == "pain":
        m = (im != TRANSP).any(axis=2)
        im[m] = im[m] * 0.6 + np.array((255, 40, 30)) * 0.4
    return img_from(im)

def item_medkit():
    im, d = new_sprite()
    d.rectangle((16, 42, 48, 63), fill=(230, 230, 230), outline=(120, 120, 120))
    d.rectangle((16, 42, 48, 45), fill=(255, 255, 255))
    d.rectangle((29, 46, 35, 60), fill=(220, 20, 20))
    d.rectangle((23, 50, 41, 56), fill=(220, 20, 20))
    return im

def item_ammo():
    im, d = new_sprite()
    d.rectangle((18, 48, 46, 63), fill=(80, 90, 50), outline=(40, 45, 25))
    for i in range(5):
        x = 20 + i * 5
        d.rectangle((x, 40, x + 3, 48), fill=(200, 160, 50))
        d.rectangle((x, 38, x + 3, 40), fill=(170, 110, 40))
    d.rectangle((22, 53, 42, 57), fill=(200, 190, 140))
    return im

def item_treasure():
    im, d = new_sprite()
    d.polygon([(22, 40), (42, 40), (38, 52), (26, 52)], fill=(230, 180, 30), outline=(150, 100, 10))
    d.rectangle((30, 52, 34, 58), fill=(210, 160, 20))
    d.rectangle((24, 58, 40, 62), fill=(230, 180, 30), outline=(150, 100, 10))
    d.rectangle((26, 42, 29, 46), fill=(255, 250, 200))
    d.ellipse((30, 44, 34, 48), fill=(220, 20, 60))
    return im

def deco_barrel():
    im = np.zeros((64, 64, 3))
    im[:] = TRANSP
    for x in range(18, 46):
        t = (x - 18) / 27.0
        s = 0.55 + 0.6 * np.sin(t * np.pi)
        im[30:64, x] = np.array((60, 110, 70)) * s
        im[32:34, x] = np.array((150, 150, 150)) * s
        im[46:48, x] = np.array((150, 150, 150)) * s
        im[60:62, x] = np.array((150, 150, 150)) * s
    im[30:32, 20:44] = (40, 160, 50)
    im[30:31, 24:40] = (120, 255, 110)
    return img_from(im)

def deco_lamp():
    im, d = new_sprite()
    d.rectangle((30, 18, 34, 60), fill=(90, 90, 100))
    d.rectangle((24, 58, 40, 63), fill=(60, 60, 70))
    d.polygon([(22, 18), (42, 18), (38, 8), (26, 8)], fill=(250, 240, 170))
    d.rectangle((26, 6, 38, 8), fill=(120, 120, 130))
    return im

def deco_pillar():
    im = np.zeros((64, 64, 3))
    im[:] = TRANSP
    for x in range(22, 42):
        t = (x - 22) / 19.0
        s = 0.5 + 0.7 * np.sin(t * np.pi)
        im[4:64, x] = np.array((150, 140, 125)) * s
        im[0:5, x - 3 if x > 24 else x] = np.array((170, 160, 140)) * s
    im[0:5, 19:45] = (165, 155, 135)
    im[59:64, 19:45] = (140, 130, 115)
    return img_from(im)

sprites = [
    ("en_walk1", robot("walk1")),
    ("en_walk2", robot("walk2")),
    ("en_shoot", robot("shoot")),
    ("en_pain", robot("pain")),
    ("en_die", robot("die")),
    ("en_dead", robot("dead")),
    ("medkit", item_medkit()),
    ("ammo", item_ammo()),
    ("treasure", item_treasure()),
    ("barrel", deco_barrel()),
    ("lamp", deco_lamp()),
    ("pillar", deco_pillar()),
]

# ---------------------------------------------------------------- weapon (48x48)
def weapon(fire):
    im, d = new_sprite(48, 48)
    if fire:
        d.ellipse((12, 0, 36, 20), fill=(255, 200, 50))
        d.ellipse((17, 4, 31, 16), fill=(255, 255, 220))
    d.rectangle((20, 14, 28, 34), fill=(70, 72, 80))
    d.rectangle((21, 14, 23, 34), fill=(130, 132, 140))
    d.rectangle((16, 30, 32, 48), fill=(50, 52, 58))
    d.rectangle((18, 30, 21, 48), fill=(95, 97, 105))
    # hand
    d.ellipse((10, 36, 26, 48), fill=(200, 150, 110))
    d.ellipse((24, 38, 38, 48), fill=(190, 140, 100))
    return im

weapons = [("gun", weapon(False)), ("gun_fire", weapon(True))]

# ---------------------------------------------------------------- palette
def to_rgb_arr(x):
    return clamp(np.array(x)) if isinstance(x, np.ndarray) else np.array(x.convert("RGB"))

all_imgs = [(n, to_rgb_arr(a), False) for n, a in walls] + \
           [(n, to_rgb_arr(a), True) for n, a in sprites] + \
           [(n, to_rgb_arr(a), True) for n, a in weapons]

pix = []
for n, a, tr in all_imgs:
    p = a.reshape(-1, 3)
    if tr:
        p = p[~(p == TRANSP).all(axis=1)]
    pix.append(p)
pix = np.concatenate(pix)
side = int(np.ceil(np.sqrt(len(pix))))
canvas = np.zeros((side * side, 3), np.uint8)
canvas[:len(pix)] = pix
canvas[len(pix):] = pix[0]
q = Image.fromarray(canvas.reshape(side, side, 3)).quantize(colors=255, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
pal = np.array(q.getpalette()[:255 * 3]).reshape(-1, 3)
pal_img = Image.new("P", (1, 1))
pal_img.putpalette(list(pal.flatten()) + [0] * (768 - pal.size))

def index_of(a, tr):
    im = Image.fromarray(a).quantize(palette=pal_img, dither=Image.Dither.NONE)
    idx = np.array(im).astype(np.int32) + 1   # 0 reserved
    if tr:
        idx[(a == TRANSP).all(axis=2)] = 0
    return idx

SHADES = 16
def rgb565(r, g, b):
    return ((int(r) >> 3) << 11) | ((int(g) >> 2) << 5) | (int(b) >> 3)

out = []
out.append("// generated by tools/gen_assets.py, do not edit\n#pragma once\n#include <stdint.h>\n")
out.append(f"enum {{ TexSize = {T}, NumShades = {SHADES} }};\n")

shade = []
for s in range(SHADES):
    f = (1.0 - s / (SHADES - 1)) ** 1.4 * 0.95 + 0.05
    row = [0] + [rgb565(*(c * f)) for c in pal]
    shade.append(row)
out.append("static const uint16_t g_shade[NumShades][256] = {\n")
for row in shade:
    out.append("  {" + ",".join("0x%04x" % v for v in row) + "},\n")
out.append("};\n")

def emit(name, idx, spans):
    h, w = idx.shape
    col = idx.T.flatten()  # column-major
    out.append(f"static const uint8_t {name}[{w * h}] = {{\n")
    for i in range(0, len(col), 64):
        out.append("  " + ",".join(str(v) for v in col[i:i + 64]) + ",\n")
    out.append("};\n")
    if spans:
        sp = []
        for x in range(w):
            ys = np.nonzero(idx[:, x])[0]
            sp.append((ys[0], ys[-1] + 1) if len(ys) else (0, 0))
        out.append(f"static const uint8_t {name}_span[{w}][2] = {{" + ",".join("{%d,%d}" % s for s in sp) + "};\n")

previews = []
for n, a, tr in all_imgs:
    idx = index_of(a, tr)
    emit("g_img_" + n, idx, tr)
    rgb = np.zeros(a.shape, np.uint8)
    rgb[:] = (255, 0, 255)
    m = idx > 0
    rgb[m] = pal[idx[m] - 1]
    previews.append(rgb)

out.append("static const uint8_t* const g_walls[] = {" + ",".join("g_img_" + n for n, _ in walls) + "};\n")
out.append("enum { " + ", ".join("Wall_" + n for n, _ in walls) + ", Wall_Count };\n")
out.append("static const uint8_t* const g_sprites[] = {" + ",".join("g_img_" + n for n, _ in sprites) + "};\n")
out.append("static const uint8_t (* const g_spriteSpans[])[2] = {" + ",".join("g_img_" + n + "_span" for n, _ in sprites) + "};\n")
out.append("enum { " + ", ".join("Spr_" + n for n, _ in sprites) + ", Spr_Count };\n")

with open(sys.argv[1], "w") as f:
    f.write("".join(out))

if len(sys.argv) > 2:
    # contact sheet of all assets for checking
    sheet = Image.new("RGB", (64 * 8 * 2, 64 * 3 * 2), (40, 40, 40))
    for i, p in enumerate(previews):
        im = Image.fromarray(p).resize((p.shape[1] * 2, p.shape[0] * 2), Image.NEAREST)
        sheet.paste(im, ((i % 8) * 128, (i // 8) * 128))
    sheet.save(sys.argv[2])
print("ok", len(walls), "walls", len(sprites), "sprites")
