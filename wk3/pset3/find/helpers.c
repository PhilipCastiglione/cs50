/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 * ...Recursively :3
 */
bool search(int value, int values[], int n)
{
    if (n < 1)
    {
        // array of 0 size means nothing to be found
        return false;
    }
    
    // NB: integer division will round down, 0 indexing means this is effectively
    // +1, so this will take the middle of an odd n sized array, or upper middle
    // item of an even n sized array
    int m = n / 2;
    
    if (values[m] == value) {
        // the midpoint is the right value, win
        return true;
    }
    else if (values[m] > value)
    {
        // new subarray from values to m, previously the midpoint
        return search(value, values, m);
    }
    else
    {
        // new subarray from (bear with me):
        // values + m + 1 (bytes) - ie. the midpoint + 1 position,
        // to what was previously the "top" of the array, adjusting m down
        // where an even sized array is using the upper "middle" point
        // note that ((n % 2) - 1) evaluates to 0 for odd, -1 for even
        return search(value, values + m + 1, m + (n % 2) - 1);
    }
}

/**
 * Sorts array of n values.
 * 
 * bubbs, yo (w/ early exit optimisation)
 */
void sort(int values[], int n)
{
    // for early exit optimisation
    int swaps = -1;
    
    // this represents the upper bound of the unsorted array
    for (int i = n; i > 1; i--)
    {
        // reset swaps for each pass through
        swaps = 0;
        
        // for each index in the unsorted range excl. 0
        for (int j = 1; j < i; j++)
        {
            // compare the previous and current index, swapping if needed
            if (values[j - 1] > values[j])
            {
                int tmp = values[j];
                values[j] = values[j - 1];
                values[j - 1] = tmp;
                
                // also increment our early exit counter
                swaps++;
            }
        }
        
        // we are done if no swaps was made on a pass
        if (swaps == 0)
        {
            break;
        }
    }
}