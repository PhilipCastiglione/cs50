/**
 * dictionary.h
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
// number of nodes in our sweet trie
#define TRIE_WIDTH 27

// the fancy data structure to hold our dictionary in memory
typedef struct _trie
{
   bool is_word;
   struct _trie *nodes[TRIE_WIDTH];
}
trie;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word);

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * Deloads a particular node and it's children in "head" (is that a thing? just
 * not tail optimised...) recursive fashion. Deload traverses deep first, then
 * frees memory on the way back out.
 */
bool deload(trie *node);

/**
 * Initialized a trie node by allocating memory, defaulting is_word to false and
 * populating children nodes with null pointers.
 * 
 * I feel a bit sad that I have to use a pointer to a pointer, to malloc out to
 * my original function scope, but I can't see another way to do this?
 */
void init_node(trie **node);

/**
 * Inserts a word into into a trie by recursively mallocing and traversing nodes
 * until the final step can be reached, then setting that nodes is_word to true.
 * 
 * Gangsta.
 */
void insert(trie *node, int *word, int length, int depth);

/**
 * Checks if a word exists in our global dictionary by converting the word to
 * our expected format, then searching the trie.
 */
bool exists(const char *word);

/**
 * Searches for a word in a trie by attempting to recursively search into the
 * final node then checking whether it's a word. If at any point we can't
 * traverse because a node is NULL, we know the word is not present.
 */
bool search(trie *node, int *word, int length, int depth);

#endif // DICTIONARY_H
