//
// Created by J.A. Korten on 11/10/2024.
//

#include "gtest/gtest.h"
#include "../TemperatureConversion.hpp"

TEST(ConverterTest, CelsiusToKelvin) {
ASSERT_NEAR(degreesCToKelvin(0.0), 273.15, 0.001);
ASSERT_NEAR(degreesCToKelvin(100.0), 373.15, 0.001);
ASSERT_NEAR(degreesCToKelvin(-273.15), 0.0, 0.001);
}