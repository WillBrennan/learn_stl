#pragma once

#include <cstdint>

namespace learn {
template <std::size_t Len, std::size_t Align>
struct aligned_storage {
    struct type {
        alignas(Align) unsigned char data[Len];
    };
};
}  // namespace learn