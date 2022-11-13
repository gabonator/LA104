youtube-dl 9myCC7e3gjo -o bigbuckbunny.mp4
ffmpeg -i bigbuckbunny.mp4 -vf "scale=w=320:h=208:force_original_aspect_ratio=increase,crop=320:208,transpose=dir=1" -r 7 -vcodec rawvideo -f rawvideo -pix_fmt rgb565be "Big buck bunny.vid"
