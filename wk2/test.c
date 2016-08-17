#include <stdio.h>

int main(void)
{
  int n = 5;
  printf("n is initially: %d\n", n);

  //for(int i = 0, n = 2; i < 1; i++)
  for(int i = 0; i < 1; i++)
  {
    // printf("n after re initialization in the loop: %d\n", n);
    n = 7;
    printf("n after secondary initialization in the loop: %d\n", n);
  }

  printf("n is finally: %d\n", n);
  // printf("we got no i: %d\n", i);

  return 0;
}
