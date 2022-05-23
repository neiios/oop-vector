#include <iostream>
#include <vector>
#include "Timer.hpp"
#include "Vector.hpp"

int main(int argc, char const* argv[]) {
  Timer t;

  double myPartTime, myTotalTime = 0;
  for (size_t i = 10000; i <= 100000000; i *= 10) {
    Vector<int> myVector;
    int customCapacityCounter = 0;

    for (size_t j = 0; j < i; j++) {
      myVector.push_back(j);
      if (myVector.size() == myVector.capacity()) {
        customCapacityCounter++;
      }
    }

    myPartTime = t.elapsed();
    myTotalTime += myPartTime;
    std::cout << "My vector's " << i << " push_back time: " << myPartTime
              << " s\n"
              << "Capacity changed: " << customCapacityCounter << " times\n";
    t.reset();
  }
  std::cout << "My vector's full time: " << myTotalTime << "\n\n";

  t.reset();
  double stdPartTime, stdTotalTime = 0;
  for (size_t i = 10000; i <= 100000000; i *= 10) {
    std::vector<int> stdVector;
    int customCapacityCounter = 0;

    for (size_t j = 0; j < i; j++) {
      stdVector.push_back(j);
      if (stdVector.size() == stdVector.capacity()) {
        customCapacityCounter++;
      }
    }

    stdPartTime = t.elapsed();
    stdTotalTime += stdPartTime;
    std::cout << "std::vector's " << i << " push_back time: " << stdPartTime
              << " s\n"
              << "Capacity changed: " << customCapacityCounter << " times\n";
    t.reset();
  }
  std::cout << "std::vector's full time: " << stdTotalTime << "\n\n";

  return 0;
}
