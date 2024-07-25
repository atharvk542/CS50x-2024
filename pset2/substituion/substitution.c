#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, string argv[])
{

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must only contain letters from the alphabet.\n");
            return 1;
        }

        for (int j = i + 1; j < strlen(argv[1]); j++)
        {
            if (argv[1][j] == argv[1][i])
            {
                printf("Key cannot have duplicate letters.\n");
                return 1;
            }
        }
    }

    string key = argv[1];
    string pText = get_string("plaintext: ");

    int l = strlen(pText);
    char cText[l];
    memset(cText, 0, l);

    for (int i = 0; i < strlen(pText); i++)
    {
        int aIndex = 0;
        for (int j = 0; j < 26; j++)
        {

            if (alphabet[j] == toupper(pText[i]))
            {
                aIndex = j;
            }
        }

        char temp[2];
        temp[1] = '\0';

        if (!(isalpha(pText[i])) && pText[i] != '\0')
        {
            temp[0] = pText[i];
            strcat(cText, temp);
        }
        else if (isupper(pText[i]))
        {
            temp[0] = toupper(key[aIndex]);
            strcat(cText, temp);
        }
        else
        {
            temp[0] = tolower(key[aIndex]);
            strcat(cText, temp);
        }
    }

    printf("ciphertext: %s\n", cText);
    return 0;
}
