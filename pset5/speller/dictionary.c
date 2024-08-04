// Implements a dictionary's functionality

#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 456976;
unsigned int counter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char wordCopy[strlen(word) + 1];
    for (int i = 0; i < strlen(word); i++)
    {
        wordCopy[i] = tolower(word[i]);
    }
    wordCopy[strlen(word)] = '\0';

    int ind = hash(word);
    node *current = table[ind];

    while (current != NULL)
    {
        if (strcmp(current->word, wordCopy) == 0)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long val = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        val += tolower(word[i]);
    }

    int ind = val * val % 456976;
    return ind;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Unable to open dictionary\n");
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(source, "%s", word) != EOF)
    {
        counter++;
        long index = hash(word);
        node *w = malloc(sizeof(node));
        if (w == NULL)
        {
            printf("Unable to create node, no memory left\n");
            unload();
            return false;
        }

        strcpy(w->word, word);

        node *b = table[index];

        if (b == NULL)
        {
            table[index] = w;
            w->next = NULL;
        }
        else
        {
            w->next = table[index];
            table[index] = w;
        }
    }

    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *tmp = NULL;
    node *cursor = NULL;
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
