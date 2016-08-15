#include <stdio.h>

int main(void)
{
  //for (int i = 0; i < 5; i++)
  for (int i = 0; i < 5 - 1; i++)
  {
    printf("%d\n", i);
  }

  float f = 1.20001;
  f *= 100;
  int i = (int) f;

  printf("yo %d\n", i);
}
