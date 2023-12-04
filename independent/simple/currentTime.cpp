#include <iostream>
#include <chrono>
#include <math.h>
#include <iomanip>

int main()
{
  auto currentTime = std::chrono::system_clock::now();
  std::time_t crrtime = std::chrono::system_clock::to_time_t(currentTime);

  std::cout << "current time: " << std::ctime(&crrtime) << std::endl;
  std::cout << "pi: " << std::setprecision(4) << 2.999999 << std::endl;
  return 0;
}
