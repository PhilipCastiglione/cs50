#include <stdio.h>
#include <stdlib.h>

#define THING 100

int n = 3;

void sweet_function(int ***ptr, int size)
{
  for (int i = 0; i < size; i++)
  {
    // this won't get hit on the first loop as size is 0
    for (int j = 0; j < n; j++)
    {
      for (int k = 0; k < n; k++)
      {
        printf("%d\n", ptr[i][j][k]);
      }
    }
  } 


  for (int i = 0; i < n; i++)
  {
    //ptr[size] = malloc(n * n * sizeof(int*));
    for (int j = 0; j < n; j++)
    {
      //ptr[size][i] = malloc(n * sizeof(int*));
      ptr[size][i][j] = size;
      printf("%d\n", ptr[size][i][j]);
    }
  }
}

int main(void)
{
  int ***hi = malloc(THING * n * n * sizeof(int*));
  for (int i = 0; i < THING; i++)
  {
    hi[i] = malloc(n * n * sizeof(int*));
    for (int j = 0; j < n; j++)
    {
      hi[i][j] = malloc(n * sizeof(int*));
    }
  }

  int size = 0;
  while (size < 5)
  {
    sweet_function(hi, size);
    size++;
  }

  return 0;
}
