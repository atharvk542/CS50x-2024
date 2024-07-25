#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover name.raw\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Cannot find card");
        return 2;
    }

    BYTE buffer[512];
    char filename[8];
    int counter = 0;
    FILE *current = NULL;

    while (fread(&buffer, 512, 1, card) == 1)
    {
        if (buffer[0] == 0xff && buffer[2] == 0xff && buffer[1] == 0xd8 &&
            (buffer[3] & 0xe0) == 0xe0)
        {
            if (current != NULL)
            {
                fclose(current);
            }

            sprintf(filename, "%03i.jpg", counter);
            current = fopen(filename, "w");
            counter++;
        }

        if (current != NULL)
        {
            fwrite(&buffer, 512, 1, current);
        }
    }

    if (current != NULL)
    {
        fclose(current);
    }

    fclose(card);
    return 0;
}
