#include <iostream>
#include <string>
#include <memory>

// Youtube channel: The Cherno
// SMART POINTERS in C++(std::unique_ptr, std::shared_ptr, std::weak_ptr)

/**
 * Smart pointers mean that when you call new, you don't have 
 * to call delete manually.
 * Smart pointers are a wrapper around a raw pointer. When you create a
 * smaert pointer, and you "make" it, it will call new and allocate the
 * memory for you, and then based on which smart pointer you use, that
 * memory will at some point be automatically freed.
*/
class Entity
{
public:
  Entity()
  {
    std::cout << "Created Entity!" << std::endl;
  }

  ~Entity()
  {
    std::cout << "Destroyed Entity!" << std::endl;
  }

  void print() { std::cout << "abcdefg\n"; }
};

/** unique_ptr
 * Unique pointer is a scoped pointer, meaning that when that pointer
 * goes out of scope, it will get destroyed and it will call delete.
 * You can't "copy" a unique pointer because when you "copy" a unique
 * pointer, basically you'll have two unique pointers pointing to the
 * same block of memory. And when one of them dies, it will free that
 * memory, meaning that the second unique pointer you had pointed to
 * the same block of memory is pointing to memory that's been freed
*/
void func1()
{
  std::cout << "outside of scope loc 1\n";
  {
    std::unique_ptr<Entity> entity = std::make_unique<Entity>();
    //or
    // std::unique_ptr<Entity> entity2(new Entity()); // avoid to call new because of exception safety
    // std::unique_ptr<Entity> e0 = entity; // shibai, unique pointer不允许被复制
    entity->print();
  }
  std::cout << "outside of scope loc 2\n";
}

/**
 * If you want to copy that pointer, kind of "share" that pointer, maybe 
 * pass it into a function, or have another class story, you're gonna
 * have a problem.
 * The way that shared pointer works is via reference counting. Reference
 * counting is basically a practice where you keep track of how many
 * references you have to your pointer. As soon as that reference count
 * reaches 0, that's when it gets deleted.
*/
void func2()
{
  std::cout << "outside of scope loc1\n";
  {
    std::shared_ptr<Entity> e0;
    std::cout << "outside of scope loc 2\n";
    {
      std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>();
      // do not use std::shared_ptr<Entity> sharedEntity(new Entity());
      /**
       * because shared pointer has to allocate another block of memory called
       * the control block where it stores that reference count. If you first
       * create a new Entity, and then pass it into a shared pointer constructor,
       * that's two allocations, because you're constructing the Entity first,
       * and then the shared pointer has to construct its control block. If you
       * use make_shared, you actually construct them together, which is a lot
       * more efficient.
      */
      e0 = sharedEntity;
    } // 离开这个scope的时候，Entity对象还未被销毁，因为e0复制了它
    std::cout << "outside of scope loc 3\n";
  } // 直到离开这个scope，Entity 才被销毁
  std::cout << "outside of scope loc4\n";
}

/**
 * When you assign a shared pointer to another shared pointer, that's copying
 * it, it will increase the reference count. But when you assign a shared
 * pointer to a weak pointer, it won't increase the reference count.
 * It's great for if you don't want to take ownership of the Entity, like
 * you might be storing a list of Entity(s), and you don't really care if
 * they're valid or not, but you just want to store like a reference to them.
 * With weak pointer, it's kind of asking it: is this still even alive? If
 * it is, you can do whatever you need to do. BUT IT WON'T KEEP IT ALIVE.
*/

void func3()
{
  std::cout << "outside of scope loc1\n";
  {
    std::weak_ptr<Entity> e0;
    std::cout << "outside of scope loc 2\n";
    {
      std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>();
      e0 = sharedEntity;
    } // 离开这个scope之后，Entity对象被销毁，因为e0没有keep it alive
    std::cout << "outside of scope loc 3\n";
  }
  std::cout << "outside of scope loc4\n";
}

int main()
{
  std::cout << "~~~~~~~~~~~Unique pointer~~~~~~~~~~~" << std::endl;
  func1();
  std::cout << "~~~~~~~~~~~Shared pointer~~~~~~~~~~~" << std::endl;
  func2();
  std::cout << "~~~~~~~~~~~Weak pointer~~~~~~~~~~~" << std::endl;
  func3();
  return 0;
}