#include <iostream>
#include <memory>
#include <typeinfo>
#include <vector>

int main()
{
  std::shared_ptr<int> sharedPtrToInt;
  std::cout << typeid(int).name() << std::endl;
  std::cout << typeid(decltype(*sharedPtrToInt)).name() << std::endl;

  if (std::is_same<decltype(*sharedPtrToInt), int>::value)
    std::cout << "is same!\n";
  else
    std::cout << "not the same!\n";

  std::vector<std::string> strVec = {"abc", "def", "ghi"};
  std::vector<std::string> strVec2 = {"ccccccc", "ddddd"};
  std::vector<double> dbVec = {1.1, 2.2, 3.3};
  double abcdefg = 882.5;

  if (std::is_same<decltype(strVec), decltype(abcdefg)>::value)
    std::cout << "is same 1!\n";
  else
    std::cout << "not the same 1!\n";

  if (std::is_same<decltype(strVec), decltype(strVec2)>::value)
    std::cout << "is same 2!\n";
  else
    std::cout << "not the same 2!\n";
  
  if (std::is_same<decltype(strVec), decltype(dbVec)>::value)
    std::cout << "is same 3!\n";
  else
    std::cout << "not the same 3!\n";

  return 0;
}
