/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

// globally declaring trie root node ptr and dict size
trie *t;
unsigned int t_size = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    return exists(word);
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // safely load file, or return false
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    
    // initialize global trie root node
    init_node(&t);
    
    // vars to keep track of the current word
    int trie_word[LENGTH + 1];
    int idx = 0;
    
    // iterate through file char by char
    for (int c = fgetc(dict); c != EOF; c = fgetc(dict))
    {
        if (c == '\n')
        {
            // add the just completed word to the trie
            insert(t, trie_word, idx, 0);
            idx = 0;
        }
        else
        {
            // set the current letter value to it's eventual array index
            // apostrophes are an edge case, put them at the end
            trie_word[idx] = (c == '\'') ? 26 : c - 97;
            idx++;
        }
    }
    
    // close the file
    fclose(dict);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return t_size;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // call deload on the root node pointer, then deal with the annoying root
    if (deload(t))
    {
        free(t);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Deloads a particular node and it's children in "head" (is that a thing? just
 * not tail optimised...) recursive fashion. Deload traverses deep first, then
 * frees memory on the way back out.
 */
bool deload(trie *node)
{
    // if the node is a NULL pointer, bail with early return
    // either: something has gone wrong, or the dict isn't loaded
    if (node == NULL)
    {
        return false;
    }
    
    // track any deload failures
    bool deloaded = true;
    
    // for each node that isn't NULL, deload it recursively
    for (int i = 0; i < TRIE_WIDTH; i++)
    {
        if (node->nodes[i] != NULL)
        {
            if (!deload(node->nodes[i]))
            {
                deloaded = false;
            }
            else
            {
                free(node->nodes[i]);
            }
        }
    }
    
    // a single deload failure bubbles up
    return deloaded;
}

/**
 * Initialized a trie node by allocating memory, defaulting is_word to false and
 * populating children nodes with NULL pointers.
 * 
 * I feel a bit sad that I have to use a pointer to a pointer, to malloc out to
 * my original function scope, but I can't see another way to do this?
 */
void init_node(trie **node)
{
    *node = malloc(sizeof(trie));
    if (node == NULL)
    {
        // on a malloc fail, panic!
        unload();
        exit(1);
    }
    
    (*node)->is_word = false;
    for (int i = 0; i < TRIE_WIDTH; i++)
    {
        (*node)->nodes[i] = NULL;
    }
}

/**
 * Inserts a word into into a trie by recursively mallocing and traversing nodes
 * until the final node is reached, then setting that node's is_word to true.
 * 
 * Gangsta.
 */
void insert(trie *node, int *word, int length, int depth)
{
    if (depth == length)
    {
        node->is_word = true;
        t_size++;
    }
    else
    {
        // if the next node step points to NULL, initialize a node there
        if (node->nodes[word[depth]] == NULL)
        {
            init_node(&(node->nodes[word[depth]]));
        }
    
        // recurse into the next node
        insert(node->nodes[word[depth]], word, length, depth + 1);
    }
}

/**
 * Checks if a word exists in our global dictionary by converting the word to
 * our expected format, then searching the trie.
 */
bool exists(const char *word)
{
    // convert the word into the format our trie expects and drop the sentinel
    int length = strlen(word);
    int trie_word[length];
    for (int i = 0; i < length; i++)
    {
        trie_word[i] = (word[i] == '\'') ? 26 : tolower(word[i]) - 97;
    }
    
    // search for the transformed word
    return search(t, trie_word, length, 0);
}

/**
 * Searches for a word in a trie by attempting to recursively search into the
 * final node then checking whether it's a word. If at any point we can't
 * traverse because a node is NULL, we know the word is not present.
 */
bool search(trie *node, int *word, int length, int depth)
{
    // if we are at the end of the proposed word
    if (depth == length)
    {
        // return whether or not it's a word
        return node->is_word;
    }
    else
    {
        if (node->nodes[word[depth]] == NULL)
        {
            // if the next node step points to NULL, the word can't be found
            return false;
        }
        else
        {
            // recurse into the next node
            return search(node->nodes[word[depth]], word, length, depth + 1);
        }
    }
}