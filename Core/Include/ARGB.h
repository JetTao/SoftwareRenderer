#pragma once

#include <cstdint>
#include "Math.hpp"

struct ARGB {
    union {
        struct {
            uint8_t b;
            uint8_t g;
            uint8_t r;
            uint8_t a;
        };
        uint32_t argb;        
    };

    ARGB() = default;

    ARGB(uint8_t inB, uint8_t inG, uint8_t inR, uint8_t inA)
        : b(inB)
        , g(inG)
        , r(inR)
        , a(inA) {}

    explicit ARGB(uint32_t inARGB)
        : argb(inARGB) {};
        
};

inline Color4f ARGBToColor4f(const ARGB &argbColor)
{
    return Color4f{
        argbColor.r / 255.0f,
        argbColor.g / 255.0f,
        argbColor.b / 255.0f,
        argbColor.a / 255.0f
    };
}

inline Color3f RGBToColor3f(const ARGB &argbColor)
{
    return Color3f{ 
        argbColor.r / 255.0f,
        argbColor.g / 255.0f,
        argbColor.b / 255.0f 
    };
}

inline ARGB Color3fToRGB(const Color3f &color3)
{
    return ARGB{ 
        static_cast<uint8_t>(color3.b * 255.0f),
        static_cast<uint8_t>(color3.g * 255.0f),
        static_cast<uint8_t>(color3.r * 255.0f),
        static_cast<uint8_t>(255)
    };
}

inline ARGB Color4fToARGB(const Color4f &color4)
{
    return ARGB{
        static_cast<uint8_t>(color4.b * 255.0f),
        static_cast<uint8_t>(color4.g * 255.0f),
        static_cast<uint8_t>(color4.r * 255.0f),
        static_cast<uint8_t>(color4.a * 255.0f)
    };
}

const ARGB RED  { 0xFFFF0000 };
const ARGB GREEN{ 0xFF00FF00 };
const ARGB BLUE { 0xFF0000FF };
const ARGB WHITE{ 0xFFFFFFFF };
