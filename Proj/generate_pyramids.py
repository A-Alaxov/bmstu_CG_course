from math import cos, pi, sin
from random import randint

def generate_pyramid(n, center_x, center_y):
    if (n < 3):
        return

    vertexes = []
    x_start, y_start, z = 1, 0, 0
    vertexes.append((x_start, y_start, z))

    angle = pi * 2 / n
    x_min, y_min, x_max, y_max = x_start, y_start, x_start, y_start
    for i in range(n - 1):
        x = cos(angle)
        y = sin(angle)

        if x < x_min:
            x_min = x
        elif x > x_max:
            x_max = x

        if y < y_min:
            y_min = y
        elif y > y_max:
            y_max = y

        vertexes.append((x, y, 0))
        angle += pi * 2 / n
    
    xmid = (x_max + x_min) / 2
    ymid = (y_max + y_min) / 2
    with open(f"pyramid_{n}.txt", "w") as f:
        f.write(f"{center_x} {center_y} 0\n")
        f.write(f"{n + 2}\n")
        for vertex in vertexes:
            f.write(f"{vertex[0]} {vertex[1]} {vertex[2]}\n")
        f.write(f"{xmid} {ymid} 1\n")
        f.write(f"{xmid} {ymid} -1\n")

        f.write(f"{2 * n}\n")
        for i in range(n):
            f.write(f"{i % n} {(i + 1) % n} {n}\n")
            f.write(f"{i % n} {(i + 1) % n} {n + 1}\n")

if __name__ == "__main__":
    n = int(input("Enter number of vertexes: "))
    center_x = int(input("center x: "))
    center_y = int(input("center y: "))

    generate_pyramid(n, center_x, center_y)
