from cs50 import get_int

# get height
height = get_int("Height: \n")
while height < 1 or height > 8:
    height = get_int("Input valid height\n")

# print #'s out
for i in range(height):
    for x in range(height - 1 - i):
        print(" ", end="")
    for y in range(i + 1):
        print("#", end="")
    print("  ", end="")
    for x in range(i + 1):
        print("#", end="")
    print()