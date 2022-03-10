#pragma once

#include <vector>
#include <iostream>

#include "Mesh.h"
#include "Texture.h"

#include "../../Shaders/BlinPhongShader.h"


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

	std::shared_ptr<BlinPhongShader> GetShader() const
	{
		return mpShader;
	}

	inline void SetShader(std::shared_ptr<BlinPhongShader> pShader)
	{
		mpShader = pShader;
	}

private:

	std::shared_ptr<Mesh> mpMesh{ nullptr };

	std::shared_ptr<Texture> mpDiffuseMap{ nullptr };

	std::shared_ptr<Texture> mpNormalMap{ nullptr };

	std::shared_ptr<Texture> mpSpecularMap{ nullptr };

	std::shared_ptr<BlinPhongShader> mpShader{ nullptr };

};
