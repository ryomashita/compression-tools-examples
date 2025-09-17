#include "add.h"

#include <gtest/gtest.h>

TEST(AddTest, AddTwoPositive) { EXPECT_EQ(add(1, 2), 3); }