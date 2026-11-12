// filter.cpp
#include "filter.hpp"

float Filter::apply(float value) const {
    return value * FILTER_COEFFICIENT;
}
