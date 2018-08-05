#pragma once
#include <cstdint>

typedef uint8_t byte;

typedef struct color32_s
{
    bool operator!=(const struct color32_s &other) const;
    byte r, g, b, a;
    inline unsigned *asInt(void) { return reinterpret_cast<unsigned*>(this); }
    inline const unsigned *asInt(void) const { return reinterpret_cast<const unsigned*>(this); }

    inline void Copy(const color32_s &rhs) {
        *asInt() = *rhs.asInt();
    }
} color32;

inline bool color32::operator!=(const color32 &other) const
{
    return r != other.r || g != other.g || b != other.b || a != other.a;
}
