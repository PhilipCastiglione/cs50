#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_caesar(string, int);

int main(int argc, string argv[])
{
    // ensure the user provided a single command line argument equivalent to
    // a non-negative int. atoi will convert silly input to 0.
    if (argc != 2 || atoi(argv[1]) < 0)
    {
        printf("Please enter a single command line argument, that is a non-negative integer.");
        return 1;
    }
    int key = atoi(argv[1]);

    // get the message to "encrypt" from the user
    printf("Enter a message to super securely encrypt: ");
    string message = GetString();

    // encode and print the message
    print_caesar(message, key);

    return 0;
}

void print_caesar(string message, int key)
{
    // for each character in the string...
    for(int i = 0, n = strlen(message); i < n; i++)
    {
        // if alphabetical, print the character plus the cipher key, wrapping
        // around within the upper or lower case ascii ranges. otherwise print
        // the char as provided (ie. for a space)
        char c = message[i];
        if (isalpha(c))
        {
            int offset = (isupper(c))? 65 : 97;
            printf("%c", (c - offset + key) % 26 + offset);
        }
        else
        {
            printf("%c", c);
        }
    }
    printf("\n");
}
