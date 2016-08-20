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
        // if the current char isn't a space and either the current char is the
        // first one, or the previous char was a space, print it
        if (name[i] != ' ' && (i == 0 || name[i - 1] == ' '))
        {
            printf("%c", toupper(name[i]));
        }
    }

    printf("\n");

    return 0;
}