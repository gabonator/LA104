import mini
colors = [0x404080, 0x4060b0, 0x4080d0, 0x40a0ff]
for x in range(0, 320/20):
  for y in range(0, 200/20):
    i = x + y
    c = colors[i % 4]
    mini.color(c)
    mini.bar(x*20, y*20+14, x*20+18, y*20+18+14)
