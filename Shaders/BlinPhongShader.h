#pragma once

#include "BaseShader.h"

#include "../Core//Include/Vertex.h"
#include "../Core/Include/Math.hpp"
#include "../Core/Include/Texture.h"

struct BlinPhongShader : BaseShader
{

public:


    struct PointLight
    {
        Point3f Position;
        Vec3f   Ambient;
        Vec3f   Diffuse;
        Vec3f   Specular;
    };

    bool VertexShader(Vertex& vertex) override
    {
        vertex.Position = Projection * View * Model * vertex.Position;
        vertex.Normal =  MatrixInvert(Model).GetTranspose() * Vec4f{ vertex.Normal, 0.0f };
        FragPosition =  Model * vertex.Position;
        return true;
    }
    
    bool FragmentShader(Fragment& fragment) override
    {
        Color3f ambient = Light.Ambient * Color3f{ 0.1f, 0.1f, 0.1f };

        // Vec3f normal = Normalize(pNormalMap->GetTexel(fragment.TexCoord));
        Vec3f normal = Normalize((fragment.Normal));
        Vec3f lightDirection = Normalize(Light.Position - FragPosition);
        float lightStrength = std::max( Dot(normal, lightDirection), 0.0f );

        Color3f diffuse = Light.Diffuse * pDiffuseMap->GetTexel(fragment.TexCoord)
            * lightStrength;

        Vec3f viewDirection = Normalize(ViewPosition - FragPosition);
        auto halfDirection = Normalize(lightDirection + viewDirection);
        float exp = std::pow( std::max( Dot(normal, halfDirection ), 0.0f ), 64.0f );

        Vec3f specular = Light.Specular * exp * 
            pSpecularMap->GetTexel(fragment.TexCoord);
        
        fragment.Color = Color4f{ ambient + diffuse + specular, 1.0f };

        return true;        

    };

    ~BlinPhongShader() = default;
    
public:

    PointLight Light;

    Mat4f Model;

    Mat4f View;
    
    Mat4f Projection;

    Point3f FragPosition;

    Point3f ObjectPosition;

    Point3f ViewPosition;

    std::shared_ptr<Texture> pDiffuseMap;

    std::shared_ptr<Texture> pNormalMap;

    std::shared_ptr<Texture> pSpecularMap;

};


