from Vector import *
import time

if __name__ == "__main__":
    w = Vector(2, 3, 0, 1)
    begin = time.time()

    v = Vector.randVfloat(2, 0, 10)


    end = time.time()

    print(f"Average time is: {(end - begin)/10000}")