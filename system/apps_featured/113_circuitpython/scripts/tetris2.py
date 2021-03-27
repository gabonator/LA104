import mini
lcd = mini.lcd()
key = mini.key()
sys = mini.sys()

colors = [0x404040, 0x55ff55, 0xff5555, 0x5555ff]
block_square = [0b0000011001100000]
block_line = [0b0000000011110000, 0b0010001000100010]
block_l = [0b0000010001000110, 0b00000001011100000, 0b0000011000100010, 0b0000111010000000]
block_l2 = [0b0100010001100000, 0b0000001011100000, 0b0110001000100000, 0b0000111010000000]
block_z = [0b0000110001100000, 0b0010011001000000]
block_z2 = [0b0000001101100000, 0b0100011000100000]
blocks = [block_l, block_line, block_z, block_square, block_z2, block_l2]

map = []
for i in range(10):
  map.append([0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0])


counter = score = blki = blkr = blkx = blky = blkc = 0
animrow = animframe = 0
block = []

def drawmap():
  for y in range(0, len(map[0])):
    for x in range(0, len(map)):
      lcd.color(colors[map[x][y]])
      lcd.bar(100+x*12, 10+y*12+16, 100+x*12+11, 10+y*12+11+16)

def showblock(bx, by, shape, c):
  for y in range(0, 4):
    for x in range(0, 4):
      if shape & (1<<(y*4+x)):
        map[bx+x][by+y] = c

def hideblock(bx, by, shape):
  for y in range(0, 4):
    for x in range(0, 4):
      if shape & (1<<(y*4+x)):
        map[bx+x][by+y] = 0

def testblock(dx, dy, dr):
    shape = block[(blkr+dr)%len(block)]
    for y in range(0, 4):
      for x in range(0, 4):
        if shape & (1<<(y*4+x)):
          if blkx+x+dx < 0 or blkx+x+dx >= len(map) or \
             blky+y+dy < 0 or blky+y+dy >= len(map[0]) or \
             not map[blkx+x+dx][blky+y+dy] == 0:
            return False
    return True

def testblockdraw(dx, dy, dr):
    hideblock(blkx, blky, block[blkr])
    ret = testblock(dx, dy, dr)
    showblock(blkx, blky, block[blkr], blkc)
    return ret

def moveblock(dx, dy, dr):
    global blkx, blky, blkr
    hideblock(blkx, blky, block[blkr])
    blkx += dx
    blky += dy
    blkr = (blkr + dr) % len(block)
    showblock(blkx, blky, block[blkr], blkc)
    drawmap()

def newblock():
    global blki, blkx, blky, blkr, blkc, block, score
    blki = score % len(blocks)
    blkr = 0
    blkx = 3
    blky = 0
    blkc = 1 + (score % (len(colors)-1))
    score += 1
    block = blocks[blki]

def checkfullrow():
    global animframe, animrow
    for y in range(0, len(map[0])):
        full = True
        for x in range(0, len(map)):
            if map[x][y] == 0:
                full = False
                break
        if full:
            animframe = 10
            animrow = y
            return True

    return False

def pushmap():
    for y in range(animrow, 1, -1):
        for x in range(0, len(map)):
            map[x][y] = map[x][y-1]
    for x in range(0, len(map)):
        map[x][y] = 0

lcd.color(0x404040)
lcd.back(0x4040ff)
lcd.gradient(0, 14, lcd.width, lcd.height-14)
lcd.back(0xffff040)
lcd.color(0xb0b040)
lcd.gradient(100+0*12-4, 10+0*12+16, 100+len(map)*12+3, 10+len(map[0])*12+16+3)
lcd.color(0x202020)
lcd.bar(100+0*12, 10+0*12+16, 100+len(map)*12-1, 10+len(map[0])*12+16-1)
blki = -1

while True:
  key = mini.key().get()
  if key == 6:
    break

  sys.delay(50)

  if animframe > 0:
    for x in range(0, len(map)):
      map[x][animrow] = (animframe % (len(colors)-1)) + 1
    drawmap()
    animframe -= 1
    if animframe == 0:
      pushmap()
      _ = checkfullrow()
    continue

  if blki == -1:
    if not checkfullrow():
        newblock()
        if not testblock(0, 0, 0):
          break
        else:
          moveblock(0, 0, 0)
    continue

  counter = counter + 1
  if key == 1 or counter > 10:
    counter = 0
    if testblockdraw(0, 1, 0):
        moveblock(0, 1, 0)
    else:
        blki = -1

  if key == 2 and testblockdraw(0, 0, 1):
    moveblock(0, 0, 1)
  if key == 4 and testblockdraw(1, 0, 0):
    moveblock(1, 0, 0)
  if key == 3 and testblockdraw(-1, 0, 0):
    moveblock(-1, 0, 0)

print("\nGame over")
