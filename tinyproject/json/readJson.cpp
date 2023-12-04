#include "json.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
  assert(argc == 2);
  std::string jsonPath = argv[1];
  std::ifstream jsonFile(jsonPath);
  nlohmann::json data = nlohmann::json::parse(jsonFile);
  auto dataDummy = data[1];
  bool flag[2] = {false, false};

  for (int i0 = 1; i0 < dataDummy.size(); ++i0)
  {
    std::cout << "i0: " << i0 << "/" << dataDummy.size()-1 << std::endl;
    if (dataDummy[i0][0][1] == "GenBc")
    {
      for (int i1 = 1; i1 < dataDummy[i0].size(); ++i1)
      {
        std::cout << "i1: " << i1 << "/" << dataDummy[i0].size()-1 << std::endl;
        if (dataDummy[i0][i1][0][0] == "bottom" && dataDummy[i0][i1][0][1] == "MovInterpBounceBackBouzidi")
        {
          flag[0] = true;
          std::cout << dataDummy[i0][i1][0][0] << std::endl;
        }
        if (dataDummy[i0][i1][0][1] == "RotWallModel")
        {
          flag[1] = true;
          std::cout << dataDummy[i0][i1][0][0] << std::endl;
        }
        if (flag[0] && flag[1])
        {
          break;
        }
      }
      break;
    }
  }

  for (int i0 = 1; i0 < dataDummy.size(); ++i0)
  {
    if (dataDummy[i0][0][1] == "TimeSequence")
    {
      for (int i1 = 1; i1 < dataDummy[i0].size(); ++i1)
      {
        if (dataDummy[i0][i1][0][0] == "solver")
          std::cout << dataDummy[i0][i1][0] << std::endl;
      }
    }
      // std::cout << dataDummy[i0][0][1] << "!!!" << std::endl;
  }

  std::cout << "flag: " << flag[0] << " " << flag[1] << std::endl;

  return 0;
}

