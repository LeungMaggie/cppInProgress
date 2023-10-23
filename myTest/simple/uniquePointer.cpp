#include <iostream>
#include <memory>
#include <vector>

struct MyData
{
  int abc;
  double def;
  std::vector<int> vec;
  MyData() {
    std::cout << "Created MyData!" << std::endl;
  }
  ~MyData() {
    std::cout << "Destroyed MyData!" << std::endl;
  }
};

class BBBBase
{
public:
  BBBBase() {
    std::cout << "BBBBase constructor!" << std::endl;
  }
  ~BBBBase() {
    std::cout << "BBBBase destructor!" << std::endl;
  }
  virtual void print_data() = 0;
};

class BBBChild : public BBBBase
{
  MyData mData;
public:
  BBBChild(MyData& myData) : mData(myData) {
    std::cout << "BBBChild constructor!" << std::endl;
  }
  ~BBBChild() {
    std::cout << "BBBChild destructor!" << std::endl;
  }
  void print_data() {
    std::cout << "BBBChild::print_data()" << std::endl;
    std::cout << "abc: " << mData.abc << ", def: " << mData.def << std::endl;
    std::cout << "vec: ";
    for (int i = 0; i < mData.vec.size(); ++i)
    {
      std::cout << mData.vec.at(i) << " ";
    }
    std::cout << std::endl;
  }
};

class AAABase
{
public:
  AAABase() {
    std::cout << "AAABase constructor!" << std::endl;
  }
  ~AAABase() {
    std::cout << "AAABase destructor!" << std::endl;
  }

  virtual std::unique_ptr<BBBBase> create(MyData* myDataIn) = 0;
};

class AAAChild : public AAABase
{
public:
  AAAChild() {
    std::cout << "AAAChild constructor!" << std::endl;
  }
  ~AAAChild() {
    std::cout << "AAAChild destructor!" << std::endl;
  }
  std::unique_ptr<BBBBase> create(MyData* myDataIn) override
  {
    // MyData myData;
    // myData.abc = myDataIn->abc;
    // myData.def = myDataIn->def;
    // myData.vec = myDataIn->vec;
    std::cout << "AAAChild::create()" << std::endl;
    return std::make_unique<BBBChild>(*myDataIn);
  }
};

/*
main pos 1
AAABase constructor!
AAAChild constructor!
main pos 2
Created MyData!
AAAChild::create()
BBBBase constructor!
BBBChild constructor!
Destroyed MyData!
main pos 3
BBBChild::print_data()
abc: 1589, def: 10.28
vec: 5551 368354 84246 96123 
main pos 4
BBBBase destructor!
AAAChild destructor!
AAABase destructor!
*/
int main()
{
  std::cout << "main pos 1" << std::endl;
  std::shared_ptr<AAABase> aaa = std::make_shared<AAAChild>();
  std::cout << "main pos 2" << std::endl;
  std::unique_ptr<BBBBase> bbb;
  {
    MyData myData;
    myData.abc = 1589;
    myData.def = 10.28;
    myData.vec = std::vector<int>{5551, 368354, 84246, 96123};
    bbb = aaa->create(&myData);
  }
  std::cout << "main pos 3" << std::endl;
  bbb->print_data();
  std::cout << "main pos 4" << std::endl;
  return 0;
}
