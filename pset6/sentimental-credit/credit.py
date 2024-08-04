from cs50 import get_string

ans = get_string("Number: ")
if len(ans) == 13 or len(ans) == 15 or len(ans) == 16:
    pass
else:
    print("INVALID")
    exit()
try:
    int(ans)
except ValueError:
    print("INVALID")
    exit()

counter = 0
digitSum = 0
normalSum = 0
for i in ans[::-1]:
    dig = int(ans[len(ans) - counter - 1: len(ans) - counter])
    if (counter % 2 == 1):
        digitSum += (dig * 2) % 10
        if (dig * 2) / 10 >= 1:
            digitSum += int((dig * 2) / 10)
    else:
        normalSum += dig
    counter += 1

if (digitSum + normalSum) % 10 == 0:
    if len(ans) == 15 and int(ans[0: 2]) in {34, 37}:
        print("AMEX")
        exit()
    elif len(ans) == 16 and int(ans[0: 2]) in {51, 52, 53, 54, 55}:
        print("MASTERCARD")
        exit()
    elif len(ans) in {13, 16} and int(ans[0: 1]) == 4:
        print("VISA")
        exit()
    else:
        print("INVALID")
        exit()

print("INVALID")
exit()
