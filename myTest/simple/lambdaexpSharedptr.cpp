#include <iostream>
#include <functional>
#include <memory>

/*
floating.io/2017/07/lambda-shared_ptr-memory-leak/
*/

/**
 * 1
*/
// typedef void (*func)();

// int main(int argc, char** argv)
// {
//   func myfunc = [](){
//   std::cout << __PRETTY_FUNCTION__ << std::endl;
//     std::cout << "did something" << std::endl; };
//   myfunc();
// }

/**
 * 2
 * capturing something changes the datatype of the lambda
 * no longer assign it to a standard function pointer
*/
// typedef std::function<void()> func;

// int main(int argc, char** argv)
// {
//   int mynum = 42;
//   func myfunc = [mynum]() {
//     std::cout << __PRETTY_FUNCTION__ << std::endl;
//     std::cout << "the answer is " << mynum << std::endl; };
//   myfunc();
// }


/**
 * 2.1 (written by me, learnt from cpp primer 5 chap 10)
*/
// int main(int argc, char** argv)
// {
//   int mynum = 42;
//   auto f = [mynum] {
//     std::cout << __PRETTY_FUNCTION__ << std::endl;
//     std::cout << "the answer is " << mynum << std::endl; };
//   f();
// }

class MyClass
{
public:
  typedef std::function<void()> CallBack;
  void on_compute(CallBack cb)
  {
    completeCallBack = cb;
  }
  void clean_something_up()   // written by me
  {
    std::cout << "abdcefg" << std::endl;
  }
  void do_complete_call()   // written by me
  {
    completeCallBack();
  }
private:
  CallBack completeCallBack;
};

void func1()
{
  std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
  obj->on_compute([obj](){ obj->clean_something_up(); });
  obj->do_complete_call();   // written by me
}

/*
if one captures a shared pointer with a lambda, that lambda will
contain a shared pointer, pointing to the same object as the original.
It's just like manually creating two shared pointers. The copy gets 
passed around with the lambda, ensuring that there's an extra reference
to that object.

If one then stores such a lambda on the object for which it has captured
a shared pointer, you've just created a scenario where the object owns
a reference to itself, and thus can never be deleted.

To fix this, first extablish a rule: never capture a std::shared_ptr<>
in a lambda. 2 safe alternatives that one can use:
1. capture the raw pointer for the lambda to use: this has a major
downside, in that the object might be deleted before you try to reference
it, and you have no way of knowing it. At least it won't hold an extra
reference, though.
2. The better way (in the author's opinion) is to use std::weak_ptr<>
The std::weak_ptr<> object, by definition, does not hold a reference to
the target. It does, however, track the target, so you can tell if the
target was deleted.
*/
// 

void func2()
{
  std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
  std::weak_ptr<MyClass> weakObj(obj);
  obj->on_compute([weakObj](){
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    auto obj = weakObj.lock();
    if (obj) { obj->clean_something_up(); }
   });
  obj->do_complete_call();
}

int main(int argc, char** argv)
{
  func1();
  func2();
  return 0;
}


