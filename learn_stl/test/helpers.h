#pragma once

#include "learn_stl/array.h"

template <typename Value>
Value generate() = delete;

template <>
double generate<double>();

template <>
float generate<float>();

template <>
int generate<int>();

template <>
unsigned char generate<unsigned char>();

template <>
learn::array<double, 3> generate<learn::array<double, 3>>();

template <typename Value>
bool equal(const Value& lhs, const Value& rhs) = delete;

template <>
bool equal<double>(const double& lhs, const double& rhs);

template <>
bool equal<float>(const float& lhs, const float& rhs);

template <>
bool equal<int>(const int& lhs, const int& rhs);

template <>
bool equal<unsigned char>(const unsigned char& lhs, const unsigned char& rhs);

template <>
bool equal<learn::array<double, 3>>(const learn::array<double, 3>& lhs,
                                    const learn::array<double, 3>& rhs);