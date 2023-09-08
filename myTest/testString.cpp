#include <string>
#include <iostream>
#include <set>

int main()
{
  std::string abc;
  abc = "aaaaaaaawow";
  std::string abcdefg = abc + "HHHH";
  auto abcdefgsubstr = abcdefg.substr(abcdefg.size()-4);
  std::cout << "abcdefgsubstr: " << abcdefgsubstr << "\n";
  std::cout << "withoutHHH: " << abcdefg.substr(0, abcdefg.size()-4) << std::endl;
  std::set<size_t> idSet;
  
  return 0;
}
