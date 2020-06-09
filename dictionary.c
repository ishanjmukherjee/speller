// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536; /* For an explanantion of why this number was chosen, see 
www.reddit.com/r/cs50/comments/ey5q5j/pset5_speller_whyyy_int_n_65536/ */

// Hash table
node *table[N];

// Number of words in dictionary
int wordcount = 0; // will be useful in sizing up the dictionary later on //

// Hashes word to a number
// djb2 algorithm
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Useful variables
    char c = fgetc(dict);
    char neww[LENGTH + 1];

    // Iteration over every character in the dictionary
    while (c != EOF)
    {
        // neww = new word from dict
        for (int i = 0; i < LENGTH + 1; i++) // Iteration over one whole word
        {
            if (c != '\n') // '\n' = beginning of a new word
            {
                neww[i] = c;
                c = fgetc(dict);
            }
            else
            {
                neww[i] = '\0';
                wordcount++;
                c = fgetc(dict);
                break;
            }
        }

        // Hashing neww
        unsigned int hashnum = hash(neww);

        // Creating newn
        node *newn = malloc(sizeof(node));
        strcpy(newn->word, neww);
        newn->next = NULL;

        // Adding newn to hashtable
        if (table[hashnum] == NULL)
        {
            table[hashnum] = newn;
        }
        else
        {
            newn->next = table[hashnum];
            table[hashnum] = newn;
        }

    }

    fclose(dict);
    return true;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Copies "word" to "input", then lowercases "input"
    int length = strlen(word);
    char input[length];
    strcpy(input, word);
    for (int i = 0; i < length; i++)
    {
        input[i] = tolower(input[i]);
    }

    // Hashes input word
    unsigned int hashnum = hash(input);

    // Checks for input word in hash table
    bool found = false;
    for (node *tmp = table[hashnum]; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(tmp->word, input) == 0)
        {
            found = true;
            break;
        }
    }
    return found;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *tmp = table[i];
        node *next = NULL;

        while (tmp != NULL)
        {
            next = tmp->next;
            free(tmp);
            tmp = next;
        }

        table[i] = NULL;
    }
    return true;
}
