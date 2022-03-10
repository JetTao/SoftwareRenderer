#pragma once

#include <cstdint>
#include "Math.hpp"

struct RGBAColor {
    union {
        struct {
            uint8_t b;
            uint8_t g;
            uint8_t r;
            uint8_t a;
        };
        uint32_t rgba;        
    };
    RGBAColor() = default;
    RGBAColor(uint8_t inR, uint8_t inG, uint8_t inB, uint8_t inA)
        : r(inR), g(inG), b(inB), a(inA) {}
    explicit RGBAColor(uint32_t inRGBA)
        : rgba(inRGBA) {};
        
};

inline Color4f RGBAToColor4f(const RGBAColor &color)
{
    return Color4f{
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };
}

inline Color3f RGBToColor3f(const RGBAColor &color)
{
    return Color3f{ color.r / 255.0f, color.g / 255.0f, color.b / 255.0f };
}

inline RGBAColor Color3fToRGB(const Color3f &color3)
{
    return RGBAColor{ 
        static_cast<uint8_t>(color3.r * 255.0f),
        static_cast<uint8_t>(color3.g * 255.0f),
        static_cast<uint8_t>(color3.b * 255.0f),
        static_cast<uint8_t>(255.0f)
    };
}

inline RGBAColor Color4fToRGBA(const Color4f &color4)
{
    RGBAColor ret(static_cast<uint8_t>(color4.r * 255.0f),
                  static_cast<uint8_t>(color4.g * 255.0f),
                  static_cast<uint8_t>(color4.b * 255.0f),
                  static_cast<uint8_t>(color4.a * 255.0f));

    return ret;
}

const RGBAColor RED  { 0xFFFF0000 };
const RGBAColor GREEN{ 0xFF00FF00 };
const RGBAColor BLUE { 0xFF0000FF };
const RGBAColor WHITE{ 0xFFFFFFFF };
