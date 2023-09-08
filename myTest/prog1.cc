#include <iostream>

int main()
{
  int aaa = 123;
  double bbb = 0.99;

  auto thisIsSomethingStupid = [&]()
  {
    std::cout << "aaa+bbb: " << aaa+bbb << std::endl;
  };

  thisIsSomethingStupid();

  return 0;
}