#pragma once

#include <vector>
#include <iostream>

#include "Mesh.h"
#include "Texture.h"


class Model {
	
public:

	Model() = default;

	explicit Model(const fs::path& path);

	bool LoadModelFromPath(const fs::path& path);

	std::shared_ptr<Mesh> GetMesh() const
	{
		return mpMesh;
	}

	std::shared_ptr<Texture> GetDiffuse() const
	{
		return mpDiffuseMap;
	}

	std::shared_ptr<Texture> GetNormal() const
	{
		return mpNormalMap;
	}

	std::shared_ptr<Texture> GetSpecular() const
	{
		return mpSpecularMap;
	}

private:

	std::shared_ptr<Mesh> mpMesh{ nullptr };

	std::shared_ptr<Texture> mpDiffuseMap{ nullptr };

	std::shared_ptr<Texture> mpNormalMap{ nullptr };

	std::shared_ptr<Texture> mpSpecularMap{ nullptr };

public: 

	// Texture Diffuse;

	// Texture Normal;

	// Texture Specular;

};
