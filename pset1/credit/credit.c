#include <cs50.h>
#include <stdio.h>

void check(void);

int main(void)
{
    check();
}

void check(void)
{
    // Finds the number of digits in the user's entered number and assigns it to numDigs
    int numDigs = 0;
    long n = 0;
    n = get_long("Number: ");
    long copy = n;
    while (copy > 0)
    {
        copy /= 10;
        numDigs++;
    }

    // Prints invalid if the user's number isn't a valid credit card number length
    if (numDigs != 13 && numDigs != 15 && numDigs != 16)
    {
        printf("INVALID\n");
        return;
    }

    int normalSum = 0;
    int digSum = 0;

    // Multiplies every other number by 2 and finds the sum of the digits
    // Also finds the normal sum of the numbers that weren't multiplied by 2
    for (int i = numDigs; i > 0; i--)
    {
        long temp = 1;
        for (int j = 1; j < i; j++)
        {
            temp *= 10;
        }

        if (i % 2 == 1)
        {
            normalSum += (n / temp) % 10;
        }
        else
        {
            int num = ((n / temp) % 10) * 2;
            digSum += num % 10 + num / 10;
        }
    }

    // Checks to see if the card is a valid credit card number
    // Prints the associated company for the card if the card is valid
    if ((digSum + normalSum) % 10 == 0)
    {
        int firstTwo = n / (divNumber / 100);
        if ((firstTwo == 34 || firstTwo == 37) && (numDigs == 15))
        {
            printf("AMEX\n");
            return;
        }
        else if ((firstTwo == 51 || firstTwo == 52 || firstTwo == 53 || firstTwo == 54 ||
                  firstTwo == 55) &&
                 (numDigs == 16))
        {
            printf("MASTERCARD\n");
            return;
        }
        else if (firstTwo / 10 == 4 && (numDigs == 13 || numDigs == 16))
        {
            printf("VISA\n");
            return;
        }
    }

    // Prints invalid for when the number is technically valid, but doesn't match with a company
    printf("INVALID\n");
    return;
}
