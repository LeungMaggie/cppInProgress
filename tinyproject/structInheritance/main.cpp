#include "something.h"
#include "pig.h"

int main()
{
  Pig<Something> piggy("monster");
  piggy.eat(2);
  piggy.sleep(8);
  return 0;
}