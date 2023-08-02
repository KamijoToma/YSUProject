//
// Created by SkyRain on 2023/05/03.
//
#include "../SList.h"
#include <gtest/gtest.h>
using namespace CXXTP;
TEST(SListTest, TestPushBack) {
  SList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  EXPECT_EQ(list.getSize(), 3);
  EXPECT_EQ(list.front(), 1);
  EXPECT_EQ(list.back(), 3);
}

TEST(SListTest, TestPopFront) {
  SList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_front();
  EXPECT_EQ(list.getSize(), 2);
  EXPECT_EQ(list.front(), 2);
  EXPECT_EQ(list.back(), 3);
}

TEST(SListTest, TestCopyConstructor) {
  SList<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  SList<int> list2(list1);
  EXPECT_EQ(list2.getSize(), 3);
  EXPECT_EQ(list2.front(), 1);
  EXPECT_EQ(list2.back(), 3);
}

TEST(SListTest, TestAssignmentOperator) {
  SList<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  SList<int> list2;
  list2.push_back(4);
  list2.push_back(5);
  list2 = list1;
  EXPECT_EQ(list2.getSize(), 3);
  EXPECT_EQ(list2.front(), 1);
  EXPECT_EQ(list2.back(), 3);
}

TEST(SListTest, TestClear) {
  SList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.clear();
  EXPECT_EQ(list.getSize(), 0);
  EXPECT_TRUE(list.empty());
}

TEST(SListTest, TestIndexOperator){
  SList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  EXPECT_EQ(list[0], 1);
  EXPECT_EQ(list[1], 2);
  EXPECT_EQ(list[2], 3);
  EXPECT_THROW(list[3], std::out_of_range);
}