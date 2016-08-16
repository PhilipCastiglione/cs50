#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // get and validate height input
    int height;
    do
    {
        printf("Height: ");
        height = GetInt();
    }
    while (height < 0 || height > 23);
    
    // print the half pyramids
    for (int i = 0; i < height; i++)
    {
        // left side
        for (int j = 0; j < height; j++)
        {
            printf((j + 1 >= height - i)? "#" : " ");
        }
        
        // middle
        printf("  ");
        
        // right side
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
    
    return 0;
}