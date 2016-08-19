#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_invalid_key(string);
void print_vigenere(string, string);

int main(int argc, string argv[])
{
    // ensure the user provided a single command arg line that is a word, with
    // all (upper or lowercase) alphabetical characters.
    if (argc != 2 || is_invalid_key(argv[1]))
    {
        printf("Please enter a single command line argument, that is a word "
               "with all alphabetical characters.\n");
        return 1;
    }
    string key = argv[1];

    // get the message to "encrypt" from the user
    printf("Enter a message to super securely encrypt: ");
    string message = GetString();

    // encode and print the message
    print_vigenere(message, key);

    return 0;
}

int is_invalid_key(string key)
{
    // if any character is not alphabetical, return 1 as the key is invalid
    // otherwise return 0
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (!isalpha(key[i]))
        {
            return 1;
        }
    }
    return 0;
}

void print_vigenere(string message, string key)
{
    int key_length = strlen(key);
    int key_idx = 0;

    // for each character in the message...
    for (int i = 0, n = strlen(message); i < n; i++)
    {
        char c = message[i];
        if (isalpha(c))
        {
            // if alphabetical, print the character plus the value associated
            // with the cipher key letter we are at, wrapping around within the
            // upper or lower case ascii ranges
            int offset = (isupper(c))? 65 : 97;
            int key_value = toupper(key[key_idx]) - 65;
            printf("%c", (c - offset + key_value) % 26 + offset);

            // then increment the cipher letter index, wrapping it back to the
            // start index if necessary.
            key_idx++;
            key_idx %= key_length;
        }
        else
        {
            // otherwise print the char as provided (ie. for a space)
            printf("%c", c);
        }
    }
    printf("\n");
}
