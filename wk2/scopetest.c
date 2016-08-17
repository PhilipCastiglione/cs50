#include <stdio.h>

// i think a man in a video lied to me today
// he was talking about variable scope.
// he said that a variable in c is scoped to the
// function in which it is initialized. that means
// I could declare a variable in one place and
// not initialize it, then access it to initialize
// it somewhere else, and then it would be fixed
// in that scope. that seems silly. let's find out.

void dank(void);

int lol;

int main(void)
{
  lol = 5;
  printf("main %d\n", lol);

  dank();
  
  return 0;
}

void dank(void)
{
  printf("dank %d\n", lol);
}


// yeah that was totally a lie
// can't slip one past me, CS50 tutor
