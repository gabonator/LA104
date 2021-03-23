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

lcd.color(0xffffff)
while key.get() == 0:
  nx = math.sin(a)*r + cx
  ny = math.cos(a)*r + cy
  a = a + math.pi * 0.95
  if lx != 0:
    lcd.line(lx, ly, nx, ny)
  lx = nx
  ly = ny
