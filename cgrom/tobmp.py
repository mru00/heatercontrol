import struct
from PIL import Image


# http://www.sharpmz.org/mz-700/charrom.html


fr = open('MZ7CGint.ROM', 'rb')
#fr = open('MZ7CGjp.rom', 'rb')

im = Image.new("1", (9*16,9*32), 0)
x = 0
y = 0

while True:
    b = fr.read(8)
    if len(b) != 8:
        break

    for i in range(8):
        (v,) = struct.unpack("b", b[i])
        for j in range(8):
            im.putpixel( (x*9+j,y*9+i), (v<<j) & 0x80)

    x+=1
    if x == 16:
        x = 0
        y += 1
            
im.save("ch.bmp")
