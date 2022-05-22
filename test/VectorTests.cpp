#include <gtest/gtest.h>
#include "../src/Vector.hpp"

TEST(VectorTests, expectTrue) {
  EXPECT_TRUE(true);
}

TEST(VectorTests, testSquareRoot) {
  double a = 16;
  const double b = squareRoot(a);

  EXPECT_EQ(4.0, b);
}
