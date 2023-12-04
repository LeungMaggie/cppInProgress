#include <iostream>
#include <vector>

class CC
{
public:
  int dd;
  explicit CC(int d): dd(d) {}
  // CC() {}
};

struct A
{
  int aaa;
  double bbb;
  std::vector<int> vec;
  CC cc;
  // explicit A(int a, double b, std::vector<CC> vecIn, CC ccIn) : aaa(a), bbb(b), vec(vecIn), cc(ccIn) {}
  explicit A(int a, double b, std::vector<int> vecIn, CC ccIn) : aaa(a), bbb(b), vec(vecIn), cc(ccIn) {}
};

struct B : public A
{
  double ccc;

  // explicit B(int a, double b, double c) : A{a, b}, ccc(c) {}
  explicit B(int a, double b, double c): ccc(c), A{{}, {}, {}, {}}
  {
    this->aaa = a;
    this->bbb = b;
  }

};
