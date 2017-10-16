#include "helpers.h"

#include <cmath>

#include "learn_stl/array.h"

template <>
double generate<double>() {
    return 3.02;
}

template <>
float generate<float>() {
    return -2.3f;
}

template <>
int generate<int>() {
    return 2332;
}

template <>
unsigned char generate<unsigned char>() {
    return 'e';
}

template <>
learn::array<double, 3> generate<learn::array<double, 3>>() {
    return {-3.14159, 2.71828, 2.302};
}

template <>
bool equal<double>(const double& lhs, const double& rhs) {
    return std::abs(lhs - rhs) < 1E-6;
}

template <>
bool equal<float>(const float& lhs, const float& rhs) {
    return std::abs(lhs - rhs) < 1E-6;
}

template <>
bool equal<int>(const int& lhs, const int& rhs) {
    return lhs == rhs;
}

template <>
bool equal<unsigned char>(const unsigned char& lhs, const unsigned char& rhs) {
    return lhs == rhs;
}

template <>
bool equal<learn::array<double, 3>>(const learn::array<double, 3>& lhs,
                                    const learn::array<double, 3>& rhs) {
    for (int i = 0; i < 3; ++i) {
        if (!equal(lhs[i], rhs[i])) {
            return false;
        }
    }

    return true;
}