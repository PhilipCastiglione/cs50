#include <stdio.h>
#include <math.h>
#include <cs50.h>

int main(void)
{
    // get and validate user input
    float change;
    do
    {
      printf("Change: ");
      change = GetFloat();
    }
    while (change < 0);
    
    // convert change to int for accuracy
    change *= 100;
    int cents = (int) round(change);
    
    // determine coin number, then print it
    int coins = 0;
    while (cents > 0)
    {
        if (cents >= 25)
        {
            cents -= 25;
        }
        else if (cents >= 10)
        {
            cents -= 10;
        }
        else if (cents >= 5)
        {
            cents -= 5;
        }
        else
        {
            cents--;
        }
        coins++;
    }
    printf("%d\n", coins);
}
