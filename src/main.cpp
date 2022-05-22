#include <iostream>
#include "Vector.hpp"

int main(int argc, char const* argv[]) {
  int input;
  std::cout << "Please enter the number: ";
  std::cin >> input;
  std::cout << squareRoot(input);
  return 0;
}
