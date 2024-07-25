#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompts the user for number of rows
    int n = -1;
    while (n < 1 || n > 8)
    {
        n = get_int("Enter a number 1 through 8: ");
    }

    // Outer loops goes through every row
    int numSpaces = n - 1;
    for (int i = n; i > 0; i--)
    {
        // Inner loop goes through every column and checks whether to print a space or hash
        for (int j = 1; j <= (n * 2) + 2 - numSpaces; j++)
        {
            if (j <= numSpaces || j == n + 1 || j == n + 2)
            {
                printf(" ");
            }
            else if (j > numSpaces || j != n + 1 || j != n + 2)
            {
                printf("#");
            }
        }
        numSpaces--;
        printf("\n");
    }
}
