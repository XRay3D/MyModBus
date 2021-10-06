#pragma once

#include <QDebug>

#include <algorithm>
#include <array>
#include <ranges>

namespace rng = std::ranges;

namespace ByteOrder {

struct Swap2B {
    template <size_t S>
    Swap2B(std::array<uint16_t, S>& values)
    {
        for (auto& v : values) {
            union {
                uint8_t u8[2];
                uint16_t u16;
            } U { .u16 = v };
            std::swap(U.u8[0], U.u8[1]);
        }
    }
    Swap2B() { }
    Swap2B(uint16_t& val)
    {
        union {
            uint8_t u8[2];
            uint16_t u16;
        } U { .u16 = val };
        std::swap(U.u8[0], U.u8[1]);
    }
    uint16_t operator()(uint16_t& val)
    {
        union {
            uint8_t u8[2];
            uint16_t u16;
        } U { .u16 = val };
        std::swap(U.u8[0], U.u8[1]);
        return U.u16;
    }
};

struct Reverse16 {
    Reverse16() { }
    template <size_t S>
    Reverse16(std::array<uint16_t, S>& values) { rng::reverse(values); }
};

struct Reverse8 {
    Reverse8() { }
    template <size_t S>
    Reverse8(std::array<uint16_t, S>& values) { rng::reverse(reinterpret_cast<std::array<uint8_t, S * 2>&>(values)); }
};

struct NoReorder {
    NoReorder() { }
    template <size_t S>
    NoReorder(std::array<uint16_t, S>&) { }
};

template <class... ByteOrdering>
struct Pack {
};

using ABCD = NoReorder;
using BADC = Swap2B;
using CDAB = Reverse16;
using DCBA = Reverse8;

template <class T, class ByteOrdering>
void reorder(T& reg, ByteOrdering)
{
    if constexpr (std::is_same_v<NoReorder, ByteOrdering>)
        return;
    constexpr uint16_t size = sizeof(std::decay_t<T>) / 2;
    using Array = std::array<uint16_t, size>;
    union U {
        Array data16 {};
        T val;
    } u;
    u.val = reg;
    ByteOrdering(u.data16);
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

struct ByteVector : std::vector<uint8_t> {
    uint16_t packSize() const noexcept
    {
        qDebug() << __FUNCTION__ << reinterpret_cast<const PDU*>(data())->adu1.size;
        return reinterpret_cast<const PDU*>(data())->adu1.size;
    }
};

#else
class ByteVector {
    enum { MaxSize = 512 };
    uint8_t m_data[MaxSize] {};
    uint16_t m_size {};

public:
    ByteVector() { }
    auto data() noexcept { return m_data; }
    auto size() noexcept { return m_size; }
    auto data() const noexcept { return m_data; }
    auto size() const noexcept { return m_size; }

    auto begin() noexcept { return m_data + 0; }
    auto end() noexcept { return m_data + m_size; }

    auto begin() const noexcept { return m_data + 0; }
    auto end() const noexcept { return m_data + m_size; }

    auto insert(uint8_t* insert, const uint8_t* begin, const uint8_t* end) noexcept
    {
        for (; begin < end; ++begin, ++insert, ++m_size)
            *insert = *begin;
        return insert;
    }

    void resize(uint16_t size) noexcept
    {
        if (size >= MaxSize) { //throw size;
        }
        if (m_size < size)
            memset(m_data + m_size, 0, size - m_size);
        m_size = size;
    }

    void emplace_back(uint8_t byte) noexcept
    {
        m_data[m_size] = byte;
        ++m_size;
    }

    auto operator[](uint16_t i) noexcept
    {
        if (i >= MaxSize) { //throw i;
        }
        return (m_data[i]);
    }
    const auto operator[](uint16_t i) const noexcept
    {
        if (i >= MaxSize) { //throw i;
        }
        return (m_data[i]);
    }

    void clear()
    {
        memset(m_data, 0, MaxSize);
        m_size = {};
    }

    uint16_t packSize() const noexcept { return reinterpret_cast<const PDU*>(m_data)->adu1.size; }

    const PDU& pdu() const noexcept { return *reinterpret_cast<const PDU*>(m_data); }

    PDU& pdu() noexcept { return *reinterpret_cast<PDU*>(m_data); }

    uint16_t crc() const noexcept { return *reinterpret_cast<const uint16_t*>(m_data + m_size - 2); }
};

#endif
