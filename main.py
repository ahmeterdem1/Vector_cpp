from Vector import *
import time

if __name__ == "__main__":
    v = Vector(1, 2, 3, 0)
    w = Vector(2, 3, 0, 1)
    begin = time.time()

    stack = (Vector(1, 2, 3, 0) for k in range(10000))

    end = time.time()

    print(f"Average time is: {(end - begin)/10000}")