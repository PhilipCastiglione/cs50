#include <string.h>
#include <stdio.h>

int main(void)
{
    char *a1 = "derpsa";
    char *a2 = "derps";

    int b = strcmp(a1, a2);

    printf("%d\n", b);

    return 0;
}
