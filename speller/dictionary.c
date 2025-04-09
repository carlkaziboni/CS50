// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

//word count
int wordcount = 0;
bool state = false;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 678;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *n = table[hash(word)];
    while (n != NULL)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
        n = n->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //declare variables to use for hashes
    int letter1 = toupper(word[0]);
    //for more than 1 letter long words
    if (strlen(word) > 1)
    {
        int letter2 = toupper(word[1]);

        //hashsing values
        if (letter1 < 73)
        {
            return (((letter1 - 65) * 26) + (letter2 - 64));
        }
        else
        {
            return (((letter1 - 65) * 26) + (letter2 - 64)) + 1;
        }
    }
    //1 letter words
    else
    {
        //hashing values
        if (strcasecmp(word, "a"))
        {
            return 0;
        }
        else if (strcasecmp(word, "i"))
        {
            return 209;
        }
        else
        {
            return 679;
        }
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (!dict)
    {
        return false;
    }

    //putting into hash table
    char buffer[LENGTH + 1];
    while (fscanf(dict, "%s", buffer) != EOF)
    {
        if (table[hash(buffer)] == NULL)
        {
            node *n = malloc(sizeof(node));
            //if not enough space
            if (n == NULL)
            {
                unload();
                return false;
            }
            strcpy(n->word, buffer);
            n->next = NULL;
            table[hash(buffer)] = n;
        }
        else
        {
            node *n = table[hash(buffer)];
            while (n->next != NULL)
            {
                n = n->next;
            }
            node *new = malloc(sizeof(node));
            if (new == NULL)
            {
                unload();
                return false;
            }
            strcpy(new->word, buffer);
            new->next = NULL;
            n->next = new;
        }
        wordcount++;
    }
    fclose(dict);
    state = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (state)
    {
        return wordcount;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < 678; i++)
    {
        node *n = table[i];
        while (n != NULL)
        {
            node *cursor = n->next;
            free(n);
            n = cursor;
        }
    }
    return true;
}
