import mini
lcd = mini.lcd()
colors = [0x8040ff, 0x8060ff, 0x8080ff, 0x40a0ff]
for x in range(0, 320/20):
  for y in range(0, 200/20):
    i = x + y
    c = colors[i % 4]
    lcd.color(c)
    lcd.bar(x*20, y*20+14, x*20+18, y*20+18+14)
