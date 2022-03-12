#pragma once

#include "Math.hpp"

struct Vertex
{
    Point4f Position;
    Color4f Color;
    Point2f TexCoord;
    Vec3f Normal;
    float RHW;
};

struct Fragment
{
    Point2i screenPosition;
    Color4f Color;
    Point2f TexCoord;
    Vec3f Normal;
    float depth;

};