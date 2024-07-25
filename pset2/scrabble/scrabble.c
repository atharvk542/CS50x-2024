#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int scores[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int findScore(string word);

int main(void)
{
    string p1 = get_string("Player 1: ");
    string p2 = get_string("Player 2: ");

    if (findScore(p1) > findScore(p2))
    {
        printf("Player 1 wins!\n");
    }
    else if (findScore(p2) > findScore(p1))
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int findScore(string word)
{
    int sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        // Checks to see if current character is actually a letter, then adds to score.
        if (tolower(word[i]) >= 97 && tolower(word[i]) <= 122)
        {
            char lower = tolower(word[i]);
            sum += scores[lower - 'a'];
        }
    }
    return sum;
}
