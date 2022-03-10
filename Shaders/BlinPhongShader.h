#pragma once

//#include "BaseShader.h"

#include "../Core//Include/Vertex.h"
#include "../Core/Include/Math.hpp"
#include "../Core/Include/Texture.h"

#include <functional>
#include <cmath>


struct BlinPhongShader
{

public:

    struct TransformMatrix
    {
        Mat4f Model;
        Mat4f View;
        Mat4f Projection;
    };

    struct Light
    {
        Point3f Position;
        Vec3f Ambient;
        Vec3f Diffuse;
        Vec3f Specular;
    };

    Vertex& VertexShader(Vertex& in)
    {
        in.Position = mMatrices.Projection * mMatrices.View * mMatrices.Model * in.Position;
        in.RHW = 1.0f / in.Position.w;
        return in;
    }
    
    Color4f FragmentShader(Fragment& in)
    {
        /*Color3f ambient{ 0.1f, 0.1f, 0.1f };

        Vec3f normal = Normalize(in.Normal);
        Vec3f lightDirection = Normalize(mLight.Position - in.Position);
        auto alpha = Dot(normal, lightDirection);
        float lightStrength = Max(alpha , 0.0f);

        Color3f diffuse = mLight.Diffuse * mpDiffuseMap->GetTexel(in.TexCoord) 
            *lightStrength;

        auto halfDirection = Normalize(lightDirection + mViewDirection);
        auto beta = Dot(normal, halfDirection);
        float exp = std::pow( Max(beta, 0.0f ), 32.0f );

        Vec3f specular = Vec3f{ 0.3f, 0.3f, 0.3f } * exp * 
            mpSpecularMap->GetTexel(in.TexCoord);
        */

        //return Color4f{ ambient + diffuse + specular, 1.0f };

        ////return Color4f{ mpDiffuseMap->GetTexel(in.TexCoord), 0.0f};

        return Color4f{ 1.0f };

    }

    inline void SetLight(const Light& light)
    {
        mLight = light;
    }

    inline void SetTransformMatrix(const TransformMatrix& matrices)
    {
        mMatrices = matrices;
    }

    inline void SetViewDirection(const Vec3f& direction)
    {
        mViewDirection = direction;
    }

    inline void SetDiffuseMap(std::shared_ptr<Texture> pTexture)
    {
        mpDiffuseMap = pTexture;
    }

    inline void SetNormalMap(std::shared_ptr<Texture> pTexture)
    {
        mpNormalMap = pTexture;
    }

    inline void SetSpecularMap(std::shared_ptr<Texture> pTexture)
    {
        mpSpecularMap = pTexture;
    }

    virtual ~BlinPhongShader() = default;
    
private:

    Light mLight;

    TransformMatrix mMatrices;

    Vec3f mViewDirection;

    std::shared_ptr<Texture> mpDiffuseMap;

    std::shared_ptr<Texture> mpNormalMap;

    std::shared_ptr<Texture> mpSpecularMap;

    
};


