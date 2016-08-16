#include <stdio.h>
#include <math.h>

int derp(void)
{
  //return 1 < 0;
  return 0 < 1;
}

int main(void)
{
  long long a = pow(10, 13);
  long long b = pow(10, 17);

  printf("a: %lld, b: %lld\n", a, b);

  int wat = derp();
  printf("wat: %d\n", wat);

  // 4111111111111111
  // 123456789012345678
  return 0;
}
