#pragma once
#include <string>
#include <iostream>

template<typename STH1, typename STH2>
struct Animal
{
  std::string mName;
  explicit Animal(const std::string& name) : mName(name) {}
  virtual void sleep(const int& duration)
  {
    std::cout << "Animal " << mName << " is going to sleep for " << duration <<" hours!" << std::endl;
  }
};