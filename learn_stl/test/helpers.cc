#include "helpers.h"

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