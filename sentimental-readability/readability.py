import curses.ascii

# count number of letters


def countletters(phrase):
    count = 0
    for i in range(len(phrase)):
        if curses.ascii.isalpha(phrase[i]):
            count += 1
        else:
            continue
    return count


# count number of words


def countwords(phrase):
    temp = phrase.split()
    return len(temp)

# count number of sentences


def countsentences(phrase):
    count = 0
    for i in range(len(phrase)):
        if phrase[i] in (".", "?", "!"):
            count += 1
        else:
            continue
    return count


# get input
text = input("Text: ")

# variables to use in formula
letternum = countletters(text)
wordsnum = countwords(text)
sentencesnum = countsentences(text)

L = (letternum/wordsnum) * 100
S = (sentencesnum/wordsnum) * 100
index = round(0.0588 * L - 0.296 * S - 15.8)

# print result
if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")

print(letternum)
print(wordsnum)
print(sentencesnum)