from cs50 import get_string

text = get_string("Text: ")

# Count number of words, sentences, and letters in text
letters = 0
words = 0
sentences = 0

for i in text:
    if i.isalpha():
        letters += 1
    if i == " ":
        words += 1
    if i == "." or i == "?" or i == "!":
        sentences += 1

words += 1

l = (letters / words) * 100
s = (sentences / words) * 100

grade = round(0.0588 * l - 0.296 * s - 15.8)

if grade < 1:
    print("Before Grade 1")
    exit()
elif grade >= 16:
    print("Grade 16+")
    exit()
else:
    print(f"Grade {grade}")
