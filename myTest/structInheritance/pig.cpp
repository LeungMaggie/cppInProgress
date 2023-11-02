#include "pig.h"

// template<typename STH>
// void Pig<STH>::eat(const int& duration)
// {
//   std::cout << "Pig " << mName << " is going to eat for " << duration << " hours!" << std::endl;
// };

// template<typename STH>
// void Pig<STH>::sleep(const int& duration)
// {
//   std::cout << "Pig " << mName << " is going to sleep for " << duration << " hours!" << std::endl;
// }

/* 
 * not work above: because the template parent of a template class
 * is not instantiated during the compilation pass that first
 * examines the template.
 * 
 * Need to tell the compiler explicitly that the names are in fact
 * dependent on the instantiation of the parent
 * 
 * From stackoverflow:
 * template: parent class member variables not visible in inherited class
*/

template<>
void Pig<Something>::eat(const int& duration)
{
  std::cout << "Pig " << mName << " is going to eat for " << duration << " hours!" << std::endl;
};

template<>
void Pig<Something>::sleep(const int& duration)
{
  std::cout << "Pig " << mName << " is going to sleep for " << duration << " hours!" << std::endl;
}
