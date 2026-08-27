#include "gtest/gtest.h"

int add(int a, int b) {
    return a + b;
}

TEST(AdditionTest, HandlesPositiveInput) {
    EXPECT_EQ(3, add(1, 2));
    EXPECT_EQ(5, add(3, 2));
}

TEST(AdditionTest, HandlesDoublesInput) {
    EXPECT_EQ(5, add(3.0, 3.0));
}


TEST(AdditionTest, HandlesNegativeInput) {
    EXPECT_EQ(-3, add(-1, -2));
    EXPECT_EQ(1, add(-1, 2));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}