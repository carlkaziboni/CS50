from cs50 import get_float

# get floating point values
change = get_float("Change: ")
while change < 0:
    change = get_float("Input correct change: ")

# get change to whole number
change = change * 100

# get number of coins
count = 0
while change > 0:
    if change >= 25:
        change -= 25
        count += 1
    elif change < 25 and change >= 10:
        change -= 10
        count += 1
    elif change < 10 and change >= 5:
        change -= 5
        count += 1
    else:
        change -= 1
        count += 1

# print count
print(count)