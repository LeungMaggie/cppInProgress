#pragma once
#include "animal.h"
#include "something.h"

template<typename STH>
struct Pig : public Animal<STH, STH>
{
  explicit Pig(const std::string& nameOfPig) : Animal<STH, STH>(nameOfPig) {}

  void eat(const int& duration);
  void sleep(const int& duration) override;
};
