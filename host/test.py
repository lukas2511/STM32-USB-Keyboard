from time import sleep

def crc(data):
    crc = 0
    for i in data:
        crc ^= i
        for i in data:
            if crc & 1:
                crc ^= 0x91
            crc>>=1
    return crc

fifo = open("/tmp/keys.fifo","w")

empty = [0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]

keys = [
    11, # h
    4, # a
    15, # l
    15, # l
    18, # o
    44, # ' '
    26, # w
    8, # e
    15, # l
    23, # t
]

for key in keys:
    data = [0x00,0x00,key,0x00,0x00,0x00,0x00,0x00]
    fifo.write('S')
    for byte in data:
        fifo.write(chr(byte))
    fifo.write('E')
    fifo.write(chr(crc(data)))

    fifo.write('S')
    for byte in empty:
        fifo.write(chr(byte))
    fifo.write('E')
    fifo.write(chr(crc(empty)))
    fifo.write('S')
    for byte in empty:
        fifo.write(chr(byte))
    fifo.write('E')
    fifo.write(chr(crc(empty)))

    fifo.write(' ')
