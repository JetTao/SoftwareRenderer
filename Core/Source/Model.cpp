#include "../Include/Model.h"

Model::Model(const fs::path& path)
{
    LoadModelFromPath(path);
}

bool Model::LoadModelFromPath(const fs::path& path)
{

    mpMesh = std::make_shared<Mesh>(path);
    if (mpMesh)
    {
        std::cout << "Succeed to load mesh! " << std::endl;
    }
    else 
    {
        std::cout << "Failed to load mesh at " << path.string() << std::endl;
    }


    fs::path diffusePath = path.parent_path() / (path.stem().string().append("_diffuse.png"));
    if (fs::exists(diffusePath))
    {
        mpDiffuseMap = std::make_shared<Texture>(diffusePath.string());
        std::cout << "Succeed to load diffuse map! " << std::endl;
    }
    else 
    {
        std::cout << "Failed to load diffuse map at " << diffusePath.string() << std::endl;
    }

    fs::path normalPath = path.parent_path() / (path.stem().string().append("_normal.png"));
    if (fs::exists(normalPath))
    {
        mpNormalMap = std::make_shared<Texture>(normalPath.string());
        std::cout << "Succeed to load normal map! " << std::endl;
    }
    else
    {
        std::cout << "Failed to load normal map at " << normalPath.string() << std::endl;
    }

    fs::path specularPath = path.parent_path() / (path.stem().string().append("_specular.png"));
    if (fs::exists(specularPath))
    {
        mpSpecularMap = std::make_shared<Texture>(specularPath.string());
        std::cout << "Succeed to load specular map! " << std::endl;
    }
    else
    {
        std::cout << "Failed to load specular map at " << specularPath.string() << std::endl;
    }

    return true;
}