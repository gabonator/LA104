youtube-dl -f 22 9myCC7e3gjo
ffmpeg -i "Big Buck Bunny (Gran Conejo Buck) CORTOMETRAJE COMPLETO FullHD 1080p Dibujos Animados para Niños-9myCC7e3gjo.mkv" \
  -vf "transpose=dir=1" -s 208x320 -r 2 -vcodec rawvideo -f rawvideo -pix_fmt rgb565 "big buck bunny.vid"