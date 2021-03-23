import mini
import math
lcd = mini.lcd()
key = mini.key()

lcd.color(0x404040)
lcd.bar(0, 14, lcd.width, lcd.height-14)
a = 0
lx = 0
ly = 0
r = min(lcd.width, lcd.height)*0.4
cx = lcd.width/2
cy = lcd.height/2
step = 0.95
dir = -0.0003
gexc = 0
while key.get() == 0:
  gexc = (gexc + 1) % 256
  lcd.color(0xffff00+gexc - gexc*256)
  nx = cx + math.sin(a)*r
  ny = cy + math.cos(a)*r
  a = a + math.pi * step
  if step >= 1 or step <= 0.5:
    dir = -dir
  step = step + dir
  if lx != 0:
    lcd.line(lx, ly, nx, ny)
  lx = nx
  ly = ny
