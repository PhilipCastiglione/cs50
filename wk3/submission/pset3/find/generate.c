/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constant
#define LIMIT 65536

int main(int argc, string argv[])
{
    // ensure generate is called with 1 or 2 command line args
    if (argc != 2 && argc != 3)
    {
        printf("Usage: generate n [s]\n");
        return 1;
    }

    // store the number of random numbers to generate as an int
    int n = atoi(argv[1]);

    // if the optional 2nd arg was provided, use srand48 to generate the seed
    // value for drand, which establishes a starting point for the pseudorandom
    // number generation algorithm
    if (argc == 3)
    {
        srand48((long int) atoi(argv[2]));
    }
    // otherwise, just use the current time (seconds since linux epoch) as the
    // seed value
    else
    {
        srand48((long int) time(NULL));
    }

    // execute for the number of requested times stored in n, print the
    // generated pseudorandom number multiplied by the limit, ie from 0 to
    // limit
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}