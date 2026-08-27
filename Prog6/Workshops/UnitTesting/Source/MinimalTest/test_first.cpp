#include "CppUTest/TestHarness.h"

TEST_GROUP(FirstTest) {
};

TEST(FirstTest, TrueIsTrue) {
    CHECK_TRUE(true);
}

TEST(FirstTest, OnePlusOneIsTwo) {
    LONGS_EQUAL(2, 1 + 1);
}