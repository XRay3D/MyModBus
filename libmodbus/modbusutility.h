#pragma once

#include <algorithm>
#include <array>
#include <ranges>

namespace rng = std::ranges;

namespace ByteOrder {
struct Swap16 {
    Swap16() { }
    template <size_t S>
    Swap16(std::array<uint16_t, S>& values)
    {
        for (auto& v : values) {
            union {
                uint8_t u8[2];
                uint16_t u16;
            } U { .u16 = v };
            std::swap(U.u8[0], U.u8[1]);
        }
    }
    uint16_t operator()(uint16_t& val)
    {
        union {
            uint8_t u8[2];
            uint16_t u16;
        } U { .u16 = val };
        std::swap(U.u8[0], U.u8[1]);
        return val;
    }
};

struct ReverseData {
    ReverseData() { }
    template <size_t S>
    ReverseData(std::array<uint16_t, S>& values) { rng::reverse(values); }
};

using ABCD = std::tuple<>;
using BADC = std::tuple<Swap16>;
using CDAB = std::tuple<ReverseData>;
using DCBA = std::tuple<Swap16, ReverseData>;

template <class T, class... ByteOrdering>
void reorder(T& reg)
{
    constexpr uint16_t size = sizeof(std::decay_t<T>) / 2;
    using Array = std::array<uint16_t, size>;
    union U {
        Array data16 {};
        T val;
    } u;
    u.val = reg;
    (ByteOrdering(u.data16), ...);
    reg = u.val;
}

}

class ModbusUtility {
public:
    ModbusUtility();
};
