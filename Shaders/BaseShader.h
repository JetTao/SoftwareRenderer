#pragma once

#include "../Core/Include/Vertex.h"

struct BaseShader
{
    virtual bool VertexShader(Vertex& position) = 0;
    
    virtual bool FragmentShader(Fragment& fragment) = 0;

    virtual ~BaseShader() = default;
};