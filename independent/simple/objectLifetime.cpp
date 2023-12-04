#include <iostream>
#include <string>

// Youtube channel: The Cherno
// Object Lifetime in C++ (Stack/Scope Lifetimes)
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
};

/** ScopedPtr
 * A class that's a wrapper over a pointer, which upon 
 * construction, heap allocates the pointer, and then
 * upon destruction, deletes the pointer. So we can 
 * automate this new and delete thing.
 * Just like the scoped pointer: unique_ptr
*/
class ScopedPtr
{
private:
  Entity* mPtr;
public:
  ScopedPtr(Entity* ptr) : mPtr(ptr) {
    std::cout << "Created ScopedPtr!" << std::endl;
  }

  ~ScopedPtr()
  {
    delete mPtr;
    std::cout << "Destroyed ScopedPtr!" << std::endl;
  }
};

int main()
{
  /** Below prints:
   * outside scope loc 1
   * Created Entity!
   * Destroyed Entity!
   * outside scope loc 2
  */
  std::cout << "~~~~~~~~~~~~~~test 1~~~~~~~~~~~~~~\n";
  std::cout << "outside scope loc 1\n";
  {
    Entity e;
  }
  std::cout << "outside scope loc 2\n";

  /** Below prints: (will not destroy Entity(), that memory is cleaned up when the application terminates)
   * outside scope loc 1
   * Created Entity!
   * outside scope loc 2
  */
  std::cout << "~~~~~~~~~~~~~~test 2~~~~~~~~~~~~~~\n";
  std::cout << "outside scope loc 1\n";
  {
    Entity* e = new Entity();
  }
  std::cout << "outside scope loc 2\n";

  /** use ScopedPtr to automatically destroy the stack 
   * 
  */
  std::cout << "~~~~~~~~~~~~~~test 3~~~~~~~~~~~~~~\n";
  std::cout << "outside scope loc 1\n";
  {
    ScopedPtr e = new Entity(); // implicit conversion
    // or
    ScopedPtr ee(new Entity());
  }
  std::cout << "outside scope loc 2\n";
}
