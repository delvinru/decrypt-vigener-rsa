src = open("1grams-3.txt", "r")
out = open("word.txt", "w")
string = src.read().lower().split("\n")

for a in string:
    if len(a) > 5:
        out.write(a)
        out.write("\n")
