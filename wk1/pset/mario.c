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
    
    // print the pyramid
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (j + 1 >= height - i)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("#\n");
    }
}
