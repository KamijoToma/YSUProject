//
// Created by SkyRain on 2023/05/03.
//
#include "../SSet.h"
#include <gtest/gtest.h>
using namespace CXXTP;
TEST(SSetTest, SetDifference) {
  SSet<int> set1, set2;
  set1.push_back(1);
  set1.push_back(2);
  set1.push_back(3);
  set2.push_back(3);
  set2.push_back(4);
  set2.push_back(5);
  
  auto set3 = set1 - set2;

  ASSERT_EQ(set3.getSize(), 2);
  EXPECT_EQ(set3[0], 1);
  EXPECT_EQ(set3[1], 2);
}

TEST(SSetTest, SetUnion) {
  SSet<int> set1, set2;
  set1.push_back(1);
  set1.push_back(2);
  set1.push_back(3);
  set2.push_back(3);
  set2.push_back(4);
  set2.push_back(5);

  auto set4 = set1 + set2;

  ASSERT_EQ(set4.getSize(), 5);
  EXPECT_EQ(set4[0], 1);
  EXPECT_EQ(set4[1], 2);
  EXPECT_EQ(set4[2], 3);
  EXPECT_EQ(set4[3], 4);
  EXPECT_EQ(set4[4], 5);
}

TEST(SSetTest, SetIntersection) {
  SSet<int> set1, set2;
  set1.push_back(1);
  set1.push_back(2);
  set1.push_back(3);
  set2.push_back(3);
  set2.push_back(4);
  set2.push_back(5);

  auto set5 = set1.and_(set2);

  ASSERT_EQ(set5.getSize(), 1);
  EXPECT_EQ(set5[0], 3);
}
