#include <gtest/gtest.h>
#include <vector>
#include "../src/Vector.hpp"

using namespace std;

// tests push back functionality
TEST(VectorTests, testPushBack) {
  unsigned int sz = 10;
  std::vector<int> stdVector;
  Vector<int> myVector;
  for (int i = 0; i <= sz; ++i) {
    stdVector.push_back(i);
    myVector.push_back(i);
    EXPECT_EQ(stdVector[i], myVector[i]);
  }
}

// test size function
TEST(VectorTests, testSize) {
  unsigned int sz = 10;
  std::vector<int> stdVector;
  Vector<int> myVector;

  EXPECT_EQ(stdVector.size(), myVector.size());

  // fill with values
  for (int i = 0; i <= sz; ++i) {
    stdVector.push_back(i);
    myVector.push_back(i);
  }

  EXPECT_EQ(stdVector.size(), myVector.size());
}

// test assign function
TEST(VectorTests, testAssign) {
  size_t elCount = 100;

  std::vector<double> stdVector;
  Vector<double> myVector;

  stdVector.assign(elCount, 42);
  myVector.assign(elCount, 42);
  EXPECT_EQ(stdVector.size(), myVector.size());

  for (size_t i = 0; i < elCount; i++) {
    EXPECT_EQ(stdVector[i], myVector[i]);
  }
}

TEST(VectorTests, testAtFunction) {
  Vector<string> myVector;
  EXPECT_THROW(myVector.at(0), std::out_of_range);

  myVector.push_back("Pedro");
  myVector.push_back("Gonzales");
  EXPECT_EQ("Pedro", myVector.at(0));
  EXPECT_EQ("Gonzales", myVector.at(1));

  EXPECT_THROW(myVector.at(2), std::out_of_range);
  EXPECT_THROW(myVector.at(-10), std::out_of_range);
}

TEST(VectorTests, testFront) {
  Vector<int> myVector;
  myVector.push_back(34);

  EXPECT_EQ(myVector.at(0), myVector.front());
}

TEST(VectorTests, testBack) {
  Vector<char> myVector;
  myVector.push_back('a');
  myVector.push_back('z');
  myVector.push_back('v');

  EXPECT_EQ(myVector.at(2), myVector.back());
}

TEST(VectorTests, testData) {
  Vector<char> myVector;
  myVector.push_back('a');
  myVector.push_back('z');
  myVector.push_back('v');

  char* unArray = myVector.directData();
  EXPECT_EQ(unArray[0], myVector.at(0));
}

TEST(VectorTests, testReserveCapacity) {
  Vector<int> myVector;
  EXPECT_EQ(0, myVector.capacity());
  myVector.push_back(1);
  myVector.push_back(1);
  myVector.push_back(1);
  EXPECT_EQ(4, myVector.capacity());
  myVector.reserve(1011);
  EXPECT_EQ(1011, myVector.capacity());
}

TEST(VectorTests, testShrinkToFitClear) {
  Vector<long long> myVector;

  myVector.reserve(20);
  EXPECT_EQ(20, myVector.capacity());
  for (int i = 0; i < 21; i++) {
    myVector.push_back(i);
  }
  EXPECT_EQ(40, myVector.capacity());
  myVector.shrink_to_fit();
  EXPECT_EQ(21, myVector.capacity());
  EXPECT_EQ(21, myVector.size());
  myVector.clear();
  EXPECT_EQ(0, myVector.size());
}

TEST(VectorTests, testInsert) {
  Vector<long long> myVector(5, 42);
  EXPECT_EQ(5, myVector.size());

  myVector.insert(myVector.begin() + 2, 144);
  EXPECT_EQ(144, myVector.at(2));

  myVector.insert(myVector.begin() + 2, 3, 134);
  EXPECT_EQ(134, myVector.at(2));
}

TEST(VectorTests, testErase) {
  Vector<long long> myVector(100, 10);
  for (const auto& el : myVector) {
    EXPECT_EQ(10, el);
  }

  myVector.erase(myVector.begin(), myVector.end() - 3);
  EXPECT_EQ(3, myVector.size());
  EXPECT_EQ(100, myVector.capacity());

  myVector.erase(myVector.begin());
  EXPECT_EQ(2, myVector.size());
  EXPECT_EQ(100, myVector.capacity());

  EXPECT_NO_THROW(myVector.erase(myVector.begin(), myVector.end()));
  EXPECT_EQ(0, myVector.size());
}

TEST(VectorTests, testResize) {
  Vector<long long> myVector(100, 10);
  EXPECT_EQ(100, myVector.size());

  myVector.resize(5);
  EXPECT_EQ(5, myVector.size());

  myVector.resize(10, 144);
  EXPECT_EQ(144, myVector.at(9));
}

TEST(VectorTests, testSwap) {
  Vector<long long> myVector(7, 42);
  Vector<long long> myVector2(13, 77);
  myVector.swap(myVector2);
  EXPECT_EQ(13, myVector.size());
  EXPECT_EQ(77, myVector.at(0));
  EXPECT_EQ(7, myVector2.size());
  EXPECT_EQ(42, myVector2.at(0));
}

TEST(VectorTests, testComparison) {
  Vector<int> alice{1, 2, 3};
  Vector<int> bob{7, 8, 9, 10};
  Vector<int> eve{1, 2, 3};

  EXPECT_EQ(false, (alice == bob));
  EXPECT_EQ(true, (alice != bob));
  EXPECT_EQ(true, (alice < bob));
  EXPECT_EQ(true, (alice <= bob));
  EXPECT_EQ(false, (alice > bob));
  EXPECT_EQ(false, (alice >= bob));

  EXPECT_EQ(true, (alice == eve));
  EXPECT_EQ(false, (alice != eve));
  EXPECT_EQ(false, (alice < eve));
  EXPECT_EQ(true, (alice <= eve));
  EXPECT_EQ(false, (alice > eve));
  EXPECT_EQ(true, (alice >= eve));
}
