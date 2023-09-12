#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <map>

// stackoverflow.com/questions/213761/what-are-some-uses-of-template-template-parameters

template <template<class, class> class V, class T, class A>
void f(V<T, A> &v)
{
  // This can be 
  // typename V<T, A>::value_type
  // but we are pretending we don't have it
  T temp = v.back();
  v.pop_back();
  std::cout << temp << std::endl;
}


// 这个输出的符号重载函数，在很多容器中都是一样的 (例如vector, forward_list等)
// template<typename T>
// static inline std::ostream& operator<<(
//   std::ostream& out,
//   std::list<T> const& v)
// {
//   out << '[';
//   if (!v.empty())
//   {
//     for (typename std::list<T>::const_iterator i = v.begin(); ;)
//     {
//       out << *i;
//       if (++i == v.end())
//         break;
//       out << ", ";
//     }
//   }
//   out << "]";
//   return out;
// }

// 顺序容器(Sequence container)一般需要2个模板参数(template argument)：
// value type, allocator
// 即使allocator已被默认，但在模板化的operator<<重载函数中也要考虑进去：

// template<template<typename, typename> class Container, class V, class A>
// std::ostream& operator<<(std::ostream& out, Container<V, A> const& v)
// {
//   out << __PRETTY_FUNCTION__ << "\n";
//   for (auto const& obj : v)
//     out << obj << ' ';
//   return out;
// }


// std::map需要4个模板参数，所以需要另一个版本的operator<<支持
// std::pair也需要一个特殊的版本？

// with C++11 which allows variadic templates
// (thus should allow variadic template template args),
// it would be possible to have single operator<< to rule them all:

// template<typename T, template<class, class...> class C, class... Args>
// std::ostream& operator<<(
//   std::ostream& os,
//   const C<T, Args...>& objs)
// {
//   os << __PRETTY_FUNCTION__ << "\n";
//   for (auto const& obj : objs)
//     os << obj << ' ';
//   return os;
// }

// 下面这个is written by me: 效果和上面两个一样
template<template<class...> class Container, class... Args>
// template<template<typename...> typename Container, typename... Args>
// template<template<typename... _T> typename Container, typename... Args>
std::ostream& operator<<(std::ostream& out, Container<Args...>& objs)
{
  out << __PRETTY_FUNCTION__ << "\n";
  for (auto const& obj : objs)
    out << obj << ' ';
    // out << obj.first << " " << obj.second << " " << std::endl;  // 换用这一行可以打印map的内容
    // out << objs.first << " " << objs.second << " " << std::endl;  // 换用这一行&去掉for循环可以打印pair的内容
  return out;
}

int main()
{
  std::vector<float> vf{1.1, 2.2, 3.3, 4.4};
  std::cout << vf << '\n';
  std::list<char> lc{'a', 'b', 'c', 'd', 'e'};
  std::cout << lc << '\n';
  std::deque<int> di{1, 2, 3, 4, 5, 6};
  std::cout << di << '\n';

  // std::map<int, char> mic;
  // mic.insert(std::make_pair(1, 'c'));
  // mic.insert(std::make_pair(4, 'a'));
  // std::cout << mic << "\n";
  // std::vector<std::pair<char, int>> picVec{std::make_pair('a', 11), std::make_pair('b', 22), std::make_pair('c', 33)};
  // std::cout << picVec << std::endl;
  // std::pair<char, int> pci('e', 99);
  // std::cout << pci << "\n";
  return 0;
}

