#include <stdio.h>

int* wat()
{
  int derp[] = { 1 };

  return &derp;
}

int main(void)
{
  int* ting = wat();

  printf("%d yo\n", *ting);

  return 0;
}
