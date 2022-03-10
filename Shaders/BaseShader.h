#pragma once

#include "../Core/Include/Vertex.h"

struct BaseShader
{
    virtual Vertex& VertexShader(Vertex& in) = 0;
    
    virtual Color4f FragmentShader(Fragment& in) = 0;

    virtual ~BaseShader() = default;
};