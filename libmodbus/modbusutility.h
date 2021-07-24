#pragma once

#include <QDebug>

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

template <class... ByteOrdering>
struct Pack {
};

using ABCD = Pack<>;
using BADC = Pack<Swap16>;
using CDAB = Pack<ReverseData>;
using DCBA = Pack<Swap16, ReverseData>;

template <class T, class... ByteOrdering>
void reorder(T& reg, Pack<ByteOrdering...>)
{
    if constexpr (sizeof...(ByteOrdering))
        return;
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

#pragma pack(push, 1)

struct PDU {
    uint8_t address;
    union {
        struct ADU1 {
            uint8_t functionCode;
            uint8_t size;
            uint8_t data[1];
        } adu1;
        struct ADU2 {
            uint8_t functionCode;
            uint8_t data[1];
        } adu2;
    };
    uint16_t crc() { return {}; };
};

#pragma pack(pop)

#if 0

struct ByteVector : std::vector<std::byte> {
    uint16_t packSize() const noexcept
    {
        qDebug() << __FUNCTION__ << reinterpret_cast<const PDU*>(data())->adu1.size;
        return reinterpret_cast<const PDU*>(data())->adu1.size;
    }
};

#else

class ByteVector {
    std::byte m_data[512] {};
    uint16_t m_size {};

public:
    ByteVector() { }
    auto data() noexcept { return m_data; }
    auto size() noexcept { return m_size; }
    auto data() const noexcept { return m_data; }
    auto size() const noexcept { return m_size; }

    auto begin() noexcept { return m_data + 0; }
    auto end() { return m_data + m_size; }

    auto begin() const noexcept { return m_data + 0; }
    auto end() const noexcept { return m_data + m_size; }

    auto insert(std::byte* insert, const std::byte* begin, const std::byte* end) noexcept
    {
        for (; begin < end; ++begin, ++insert, ++m_size)
            *insert = *begin;
        return insert;
    }

    void resize(uint16_t size) noexcept
    {
        if (size >= 512)
            throw size;
        if (m_size < size)
            memset(m_data + m_size, 0, size - m_size);
        m_size = size;
    }

    void emplace_back(std::byte byte) noexcept
    {
        m_data[m_size] = byte;
        ++m_size;
    }

    auto operator[](uint16_t i) noexcept
    {
        if (i >= 512)
            throw i;
        return (m_data[i]);
    }
    const auto operator[](uint16_t i) const noexcept
    {
        if (i >= 512)
            throw i;
        return (m_data[i]);
    }

    void clear()
    {
        memset(m_data, 0, 512);
        m_size = {};
    }

    uint16_t packSize() const noexcept
    {
        return reinterpret_cast<const PDU*>(m_data)->adu1.size;
    }

    uint16_t crc() const noexcept
    {
        return *reinterpret_cast<const uint16_t*>(m_data + m_size - 2);
    }
};

#endif
