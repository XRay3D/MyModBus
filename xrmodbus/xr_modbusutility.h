#pragma once

#include <QDebug>

#include <algorithm>
#include <array>
#include <ranges>

namespace rng = std::ranges;

union U {
    uint16_t u16;
    uint8_t u8[2];
};

namespace ByteOrder {

struct Swap2B {
    template <size_t S>
    Swap2B(std::array<uint16_t, S>& values)
    {
        for (auto& v : values) {
            U u { .u16 = v };
            std::swap(u.u8[0], u.u8[1]);
        }
    }
    Swap2B() { }
    Swap2B(uint16_t& val)
    {
        U u { .u16 = val };
        std::swap(u.u8[0], u.u8[1]);
    }
    uint16_t operator()(uint16_t& val)
    {
        U u { .u16 = val };
        std::swap(u.u8[0], u.u8[1]);
        return u.u16;
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
    uint8_t data_[MaxSize] {};
    uint16_t size_ {};

public:
    ByteVector() { }
    auto data() const noexcept { return data_; }
    auto data() noexcept { return data_; }

    auto size() const noexcept { return size_; }
    auto size() noexcept { return size_; }

    auto begin() const noexcept { return data_; }
    auto begin() noexcept { return data_; }

    auto end() const noexcept { return data_ + size_; }
    auto end() noexcept { return data_ + size_; }

    auto insert(uint8_t* insert, const uint8_t* begin, const uint8_t* end) noexcept
    {
        size_ += end - begin;
        while (begin < end)
            *insert++ = *begin++;
        return insert;
    }

    void resize(uint16_t size) noexcept
    {
        if (size >= MaxSize) { //throw size;
        }
        if (size_ < size)
            memset(data_ + size_, 0, size - size_);
        size_ = size;
    }

    void emplace_back(uint8_t byte) noexcept
    {
        if (size_ >= MaxSize) { //throw size;
        }
        data_[size_] = byte;
        ++size_;
    }

    auto operator[](uint16_t i) noexcept
    {
        if (i >= MaxSize) { //throw i;
        }
        return (data_[i]);
    }
    const auto operator[](uint16_t i) const noexcept
    {
        if (i >= MaxSize) { //throw i;
        }
        return (data_[i]);
    }

    void clear()
    {
        memset(data_, 0, MaxSize);
        size_ = {};
    }

    uint16_t packSize() const noexcept { return reinterpret_cast<const PDU*>(data_)->adu1.size; }

    const PDU& pdu() const noexcept { return *reinterpret_cast<const PDU*>(data_); }

    PDU& pdu() noexcept { return *reinterpret_cast<PDU*>(data_); }

    uint16_t crc() const noexcept { return *reinterpret_cast<const uint16_t*>(data_ + size_ - 2); }
};

#endif
