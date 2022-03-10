#pragma once

#include "BaseShader.h"

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
        Vec3f   Ambient;
        Vec3f   Diffuse;
        Vec3f   Specular;
    };

    Vertex& VertexShader(Vertex& in)
    {
        in.Position = mMatrices.Projection * mMatrices.View * mMatrices.Model * in.Position;
        in.RHW = 1.0f / in.Position.w;
        return in;
    }
    
    Color4f FragmentShader(Fragment& in)
    {
        Color3f ambient{ 0.8f, 0.3f, 0.3f };

        /*Vec3f normal = Normalize(mpNormalMap->GetTexel(in.TexCoord));
        Vec3f lightDirection = Normalize(mLight.Position - in.Position);
        float lightStrength = Max( Dot(normal, lightDirection), 0.0f );

        Color3f diffuse = mLight.Diffuse;
            diffuse = diffuse * mpDiffuseMap->GetTexel(in.TexCoord);
        diffuse = diffuse * lightStrength;

        auto halfDirection = Normalize(lightDirection + mViewDirection);
        float exp = std::pow( Max( Dot(normal, halfDirection ), 0.0f ), 32.0f );

        Vec3f specular = Vec3f{ 0.3f, 0.3f, 0.3f } * exp * 
            mpSpecularMap->GetTexel(in.TexCoord);*/
        
        

        return Color4f{ mpDiffuseMap->GetTexel(in.TexCoord), 1.0f };

        //return Color4f{ ambient + diffuse , 1.0f };

        //return Color4f{ ambient + diffuse + specular, 1.0f };

        // return Color4f{ 1.0f };



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

// public: 

//     Texture Diffuse;
//     Texture Normal;
//     Texture Specular;
};


