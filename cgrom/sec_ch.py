import struct
from PIL import Image

fr = open('MZ7CGint.ROM', 'rb')

b = fr.read(8)
b = fr.read(8)

for i in range(8):
    (v1,) = struct.unpack("b", b[i])
    for j in range(8):
        if (v1 << j) & 0x80 != 0:
            bit = 1
        else:
            bit = 0
        print bit,
    print
            
