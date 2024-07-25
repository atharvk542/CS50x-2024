#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

double findAvgNumSentences(string passage);
double findAvgNumLetters(string passage);
int wordCounter = 0;

int main(void)
{
    string text = get_string("Text: ");
    double s = findAvgNumSentences(text);
    double l = findAvgNumLetters(text);

    double index = (0.0588 * l) - (0.296 * s) - 15.8;
    int grade = round(index);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

double findAvgNumSentences(string passage)
{
    // Initializes counters for number of words and sentences
    int senCounter = 0;
    for (int i = 1; i < strlen(passage); i++)
    {
        // Variables for current character and previous character
        char c = passage[i];
        char p = passage[i - 1];

        // Increases sentence count if punctuation is present and word count if there is a space
        if (c == '.' || c == '?' || c == '!')
        {
            wordCounter++;
            senCounter++;
        }
        else if (c == ' ' && p != '.' && p != '?' && p != '!')
        {
            wordCounter++;
        }
    }

    // Calculates and returns average sentence length per 100 words
    return ((double) senCounter / wordCounter) * 100;
}

double findAvgNumLetters(string passage)
{
    int lCounter = 0;

    for (int i = 1; i < strlen(passage); i++)
    {
        char c = passage[i];
        char p = passage[i - 1];

        if (tolower(p) > 96 && tolower(p) < 123)
        {
            lCounter++;
        }
    }

    return ((double) lCounter / wordCounter) * 100;
}
