#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("minutes: ");
    int minutes = GetInt();
    printf("bottles: %d\n", minutes * 12);
}
