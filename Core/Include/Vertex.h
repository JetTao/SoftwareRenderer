#pragma once

#include "Math.hpp"

struct Vertex
{
    Point4f Position;
    Color4f Color;
    Vec2f TexCoord;
    Vec3f Normal;
    float RHW;
};


struct Fragment
{
    Point3f Position;
    Vec2f TexCoord;
    Vec3f Normal;
};