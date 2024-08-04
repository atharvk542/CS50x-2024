from cs50 import get_int

ans = 0
while ans < 1 or ans > 8:
    ans = get_int("Enter an integer from 1 to 8, inclusive ")

for i in range(1, ans + 1):
    for j in range(1, (ans * 2) + 3):
        if j <= ans - i or j == ans + 1 or j == ans + 2:
            print(" ", end="")
        else:
            if not j > ans + 2 + i:
                print("#", end="")
    print()
