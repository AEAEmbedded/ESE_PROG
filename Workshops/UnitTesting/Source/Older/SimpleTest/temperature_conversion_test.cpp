//
// Created by J.A. Korten on 11/10/2024.
//

#include "gtest/gtest.h"
#include "TemperatureConvertor.hpp"

TEST(TemperatureConversionTest, CentrigradeToKelvin) {
  // Test cases
  ASSERT_NEAR(centrigradeToKelvin(0.0), 273.15, 0.001);
  ASSERT_NEAR(centrigradeToKelvin(100.0), 373.15, 0.001);
  ASSERT_NEAR(centrigradeToKelvin(-273.15), 0.0, 0.001);
}

TEST(TemperatureConversionTest, FailingTest) {
  // Test cases
  ASSERT_NEAR(centrigradeToKelvin(-273.15), 2.0, 0.001);
}