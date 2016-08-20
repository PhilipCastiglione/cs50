#include <stdio.h>

int main(void)
{
  // this actually compiles with errors - parens required to remove logical
  // comparison order ambiguity
  if (1 && 0 || 1)
  {
    printf("true and false or true is TRUE");
  }

  if (0 && 0 || 1)
  {
    printf("false and false or true is TRUE");
  }

  return 0;
}
