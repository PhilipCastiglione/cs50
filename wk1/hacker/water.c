#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // get and validate user input
    int minutes;
    do
    {
        printf("minutes: ");
        minutes = GetInt();
    }
    while (minutes <= 0);
    
    // print output
    printf("bottles: %d\n", minutes * 12);
}