#include <iostream>
#include <vector>
#include <algorithm>

/** lambda expression
 * 当定义一个lambda时，编译器生成一个与lambda对应的新的（未命名的）类类型
 * 当向一个函数传递一个lambda时，同时定义了一个新类型和该类型的一个对象：
 * 传递的参数就是此编译器生成的类类型的未命名对象
 * 从lambda生成的类都包含一个 对应该lambda所捕获的变量的数据成员
 * 类似任何普通类的数据成员，lambda的数据成员也在lambda对象创建时被初始化
*/

/** 值捕获
 * 被捕获变量的值是在lambda创建时拷贝，因此随后对其修改不会影响到lambda内对应的值
*/
void fcn1()
{
  size_t v1 = 42;                         // 局部变量
  auto f = [v1] { return v1; };           // 将v1拷贝到名为f的可调用对象
  v1 = 0;
  auto j = f();                           // j为42；f保存了我们创建它时v1的拷贝
  std::cout << "j = " << j << std::endl;
}


/** 引用捕获
 * 必须确保被引用的对象在lambda执行的时候时存在的
 * lambda捕获的都是局部变量，这些变量在函数结束后就不复存在了
 * 如果lambda可能在函数结束后执行，捕获的引用指向的局部变量已经消失
*/
void fcn2()
{
  size_t v1 = 42;                         // 局部变量
  auto f2 = [&v1] { return v1; };         // 对象f2包含v1的引用
  v1 = 0;
  auto j = f2();                          // j为0；f2保存v1的引用，而非拷贝
}

void biggies(std::vector<std::string>& words,
             std::vector<std::string>::size_type sz,
             std::ostream& os = std::cout,
             char c = ' ')
{
  // 重排words的代码...
  // 不能拷贝ostream对象，因此捕获os的唯一方法就是捕获其引用（或指向os的指针）
  std::for_each(words.begin(), words.end(),
                [&os, c] (const std::string& s) { os << s << c; } );
}

/** 隐式捕获
 * 让编译器根据lambda体中的代码来推断我们要使用哪些变量
 * 为了指示编译器推断捕获列表，应在捕获列表中写一个&或=
 * &告诉编译器采用"引用捕获"方式；=表示采用"值捕获"方式
 * 混合使用隐式捕获和显式捕获时，捕获列表中的第一个元素必须是&或=
 * 显式捕获的变量必须使用与隐式捕获不同的方式
*/
void biggies_implicit(std::vector<std::string>& words,
                      std::vector<std::string>::size_type sz,
                      std::ostream& os = std::cout,
                      char c = ' ')
{
  // 重排words的代码...
  // os隐式捕获，引用捕获方式
  // c显式捕获，值捕获方式
  std::for_each(words.begin(), words.end(),
                [&, c] (const std::string& s) { os << s << c; });
  
  // os显式捕获，引用捕获方式
  // c隐式捕获，值捕获方式
  std::for_each(words.begin(), words.end(),
                [=, &os] (const std::string& s) { os << s << c; });
}

/** 可变lambda
 * 默认情况下，对于一个值被拷贝的变量，lambda不会改变其值
 * 如果希望能改变一个被捕获的变量的值，就必须在参数列表首加上关键字mutable
*/
void fcn3()
{
  size_t v1 = 42;   // 局部变量
  auto f = [v1] () mutable { return ++v1; };
  v1 = 0;
  auto j = f();     // j为43
}

/** 指定lambda返回类型
 * 
*/
void fcn4(const std::vector<int>& vi)
{
  // 将一个序列中的每个负数替换为其绝对值
  // 前两个迭代器表示输入序列，第三个迭代器表示目的位置
  // 当输入迭代器和目的迭代器相同时，transform将输入序列中每个元素替换为可调用对象操作该元素得到的结果
  std::transform(vi.begin(), vi.end(), vi.begin(),
                 [] (int i) { return i < 0 ? -i : i; });
  // 若改写成看起来是等价的if语句，就会产生编译错误，原因是不能判断lambda的返回类型
  // 编译器推断这个版本的lambda返回类型为void，但它返回了一个int值
                //  [] (int i) { if (i < 0) return -i; else return i; }

  // 可以改写成：
  std::transform(vi.begin(), vi.end(), vi.begin(),
                 [] (int i) ->int
                 { if (i < 0) return -i; else return i; });
}

/** exercice
 * 
*/
void fcn5(const std::vector<std::string>& wordVec)
{
  
}


int main()
{
  std::vector<std::string> wordVec = {"could", "have", "been", "a", "rockstar"};
  std::cout << std::count_if(wordVec.begin(), wordVec.end(),
                             [] () {});
  return 0;
}