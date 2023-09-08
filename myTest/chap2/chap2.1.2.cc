#include <iostream>
#include <bitset>

void func1()
{
  bool b = 42;
  std::cout << "b: " << b << "\n";
  int i = b;
  std::cout << "i: " << i << "\n";
  i = 3.14;
  std::cout << "i: " << i << "\n";
  double pi = i;
  std::cout << "pi: " << pi << "\n";
  unsigned char c = -1;
  printf("c: %02x\n", c);
  signed char c2 = 256;
  std::cout << "c2: " << c2 << "\n";
  return;
}

void func2()
{
  unsigned u = 10;
  int i = -42;
  std::cout << i + i << std::endl;
  std::cout << u + i << std::endl;
  return;
}


int main()
{
  func2();
  return 0;
}