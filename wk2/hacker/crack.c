/**
 * crack.c - better than crack
 * 
 * This program is designed to accept a single command line arg that is a
 * password encrypted with C’s DES-based (not MD5-based) crypt function.
 * Which, as it turns out, is hilariously insecure.
 * 
 * provided assumptions:
 *  - each password is originally entirely composed of printable ASCII
 *  - each password is originally no more than 8 chars long
 * 
 * NB:
 *  - printable ascii are those in (decimal) range 32 to 126 inclusive
 *  - a non-exhaustive American dictionary has been provided at
 *    /usr/share/dict/words to facilitate a dictionary search component
 * 
 * TO COMPILE:
 * clang -o crack crack.c -lcrypt
 */

#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>

// function prototypes
void search_file(string filename);
void num_search();
void exhaustive_search();
void rotate_chars_and_test(int lower_bound, int upper_bound, string test_pw, int size, int length);
int password_matches(string test_pw);

// declare variables globally so we don't have to pass them around everywhere
// because we are lazy
string cipher;
char salt[2];
char plaintext[8];

// FOR DEBUGGING
long long counter;

int main(int argc, string argv[])
{
    // ensure crack has the right number of args
    if (argc != 2)
    {
        printf("Enter a single arg (a cipher)\n");
        return 1;
    }
    
    // store the arg in cipher and establish the salt
    cipher = argv[1];
    memcpy(salt, cipher, 2);
    
    // look in common passwords
    search_file("common");
    
    // look in the dictionary
    if (!password_matches(plaintext))
    {
        search_file("words");
    }
    
    // look in the dictionary + num
    if (!password_matches(plaintext))
    {
        search_file("words_num");
    }
    
    // look in num only
    // WARNING, 10^8 total comparisons, this runs slowly
    if (!password_matches(plaintext))
    {
        num_search();
    }
    
    // look through everything
    // WARNING, 97^8 total comparisons, this runs _slowly_
    if (!password_matches(plaintext))
    {
        exhaustive_search();
    }
    
    if (password_matches(plaintext))
    {
        printf("%s\n", plaintext);
    }
    else
    {
        printf("couldn't crack: %s\n", cipher);
    }
    
    return 0;
}

void search_file(string filename)
{
    // read in the dict file and set up line and read result for iteration
    FILE* dict = fopen(filename, "r");
    char line[255];
    int read_result;
    
    // go through every line in the text file extract and compare each token
    // copy the password and exit early if you find a match
    do
    {
        read_result = fscanf(dict, "%s", line);
        if (password_matches(line))
        {
            memcpy(plaintext, line, 8);
            break;
        }
    }
    while (read_result == 1);
    
    fclose(dict);
}

void num_search()
{
    // for 1 to 8 char lengths
    for (int i = 1; i <= 8; i++)
    {
        char test_pw[i];
        rotate_chars_and_test(48, 59, test_pw, i, i);
        if (password_matches(test_pw))
        {
            memcpy(plaintext, test_pw, i);
            break;
        }
    }
}

void exhaustive_search()
{
    // for 1 to 8 char lengths
    for (int i = 1; i <= 8; i++)
    {
        char test_pw[i];
        rotate_chars_and_test(32, 127, test_pw, i, i);
        if (password_matches(test_pw))
        {
            memcpy(plaintext, test_pw, i);
            break;
        }
    }
}
        
void rotate_chars_and_test(int lower_bound, int upper_bound, string test_pw, int size, int length)
{
    for (int k = lower_bound; k < upper_bound; k++)
    {
        printf("%lld\n", ++counter);
        if (password_matches(test_pw))
        {
            break;
        }
        test_pw[length - size] = (char) k;
        if (size !=1)
        {
            rotate_chars_and_test(lower_bound, upper_bound, test_pw, size - 1, length);
        }
    }
}

int password_matches(string test_pw)
{
    return test_pw != NULL && strcmp(crypt(test_pw, salt), cipher) == 0;
}