import numpy as np

def fake_memset(a, value):
    if(len(value) != 8):
        exit(1)

    for i in range(0, 8):
        for j in range(0 ,8):
            a[j+i*8] = value[j]

    return a


def fake_bitcpy(dest, write, src, read, cnt):
    dest[write : write+cnt] = src[read : read+cnt]
    return dest

def dump(a):
    for i in range(len(a)):
        print(a[i],end="")
    print("")

input = np.zeros(64, dtype=int)
output = np.zeros(64, dtype=int)

input = fake_memset(input,[1,0,1,0,1,1,1,1]) #0xAF

for i in range(1,33):
    for j in range(0,16):
        for k in range(0,16):
            print("\nCopy from src",j, "to dest",k,"cnt =", i)
            print("src :    ",end="")
            dump(input)
            print("dest:    ",end="")
            dump(output)

            print("{:2}:{:2}:{:2}".format(i,k,j), end=" ")
            output = fake_bitcpy(output, k, input, j, i)
            dump(output)

