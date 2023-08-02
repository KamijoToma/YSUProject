//
// Created by SkyRain on 2023/05/03.
//
#include "../SStack.h"
#include <gtest/gtest.h>
using namespace CXXTP;
TEST(SStackTest, EmptyStack) {
  SStack<int> stack;
  EXPECT_TRUE(stack.empty());
}

TEST(SStackTest, PushAndPop) {
  SStack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  EXPECT_EQ(stack.size(), 3U);
  EXPECT_EQ(stack.top(), 3);
  stack.pop();
  EXPECT_EQ(stack.top(), 2);
  stack.pop();
  EXPECT_EQ(stack.top(), 1);
  stack.pop();
  EXPECT_TRUE(stack.empty());
}
