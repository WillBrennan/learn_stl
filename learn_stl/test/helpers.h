#pragma once

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