#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // get and store a name from the user
    string name = GetString();

    // loop through the chars in name
    for(int i = 0, n = strlen(name); i < n; i++)
    {
        // if we are at the first char, or if the preceeding char was a space
        // then print the current char (as uppercase)
        if (i == 0 || name[i - 1] == ' ')
        {
            printf("%c", toupper(name[i]));
        }
    }

    printf("\n");

    return 0;
}
