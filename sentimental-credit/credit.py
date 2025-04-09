# get valid input
def getcard():
    while True:
        try:
            cardn = int(input("Card number: "))
            break
        except:
            print("Invalid input")
    return cardn

# checksum


def checksum(cardnumb):
    cardnumb = str(cardnumb)
    # basic total
    i = 0
    totalbas = 0
    while (len(cardnumb) - i - 1) >= 0:
        totalbas += int(cardnumb[len(cardnumb) - i - 1])
        i += 2
    # complicated total
    i = 1
    totalcmp = 0
    while (len(cardnumb) - i - 1) >= 0:
        temp = int(cardnumb[len(cardnumb) - i - 1]) * 2
        if temp > 9:
            totalcmp += int(temp / 10)
            totalcmp += temp % 10
        else:
            totalcmp += temp
        i += 2
    if (totalbas + totalcmp) % 10 == 0:
        return True
    else:
        return False


# actual implementation
cardnum = getcard()
while cardnum < 0:
    print("Invalid input")
    cardnum = getcard()

# checking card
tcardnum = str(cardnum)
if (len(tcardnum) == 13 or len(tcardnum) == 16) and (tcardnum[0] == "4") and (checksum(cardnum) == True):
    print("VISA")
elif (len(tcardnum) == 15) and (tcardnum[0] == "3") and (tcardnum[1] in ["4", "7"]) and (checksum(cardnum) == True):
    print("AMEX")
elif (len(tcardnum) == 16) and (tcardnum[0] == "5") and (tcardnum[1] in ["2", "1", "3", " 4", "5"]) and (checksum(cardnum) == True):
    print("MASTERCARD")
else:
    print("INVALID")