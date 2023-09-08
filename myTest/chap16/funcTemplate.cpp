#include <iostream>

// 模板类型参数 type parameter
template<typename T>
T foo(T* p)
{
  T tmp = *p;
  return tmp;
}

// 非类型模板参数 nontype parameter
template<unsigned N, unsigned M>
int compare(const char (&p1)[N], const char (&p2)[M])
{
  std::cout << p1 << "! " << p2 << std::endl;
  return p1 < p2;
}

int main()
{
  std::cout << compare("ohh", "mum") << std::endl;
  return 0;
}

