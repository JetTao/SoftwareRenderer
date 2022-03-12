#pragma once

#include "Math.hpp"
#include <vector>
#include <string>

#include <filesystem>
namespace fs = std::filesystem;  

class Mesh {
	
public:

	struct Index
	{
		Index() = default;
		Index(uint32_t positionIndex, uint32_t texCoordIndex, uint32_t normalIndex)
			: PositionIndex(positionIndex)
			, TexCoordIndex(texCoordIndex)
			, NormalIndex(normalIndex) {}

		uint32_t PositionIndex;
		uint32_t TexCoordIndex;
		uint32_t NormalIndex;
	};

	// struct TextureNames
	// {
	// 	std::string DiffuseName;
	// 	std::string SpecularName;
	// 	std::string NormalName;
	// };

	Mesh() = default;

	explicit Mesh(const fs::path& path);

	bool LoadMeshFromPath(const fs::path& path);


	inline uint32_t NumPositions() const
	{
		return mPositions.size();
	}

	inline uint32_t NumIndices() const
	{
		return mIndices.size();
	}

	inline uint32_t NumTexCoords() const
	{
		return mTexCoords.size();
	}

	inline uint32_t NumNormals() const
	{
		return mNormals.size();
	}

	inline Index GetIndex(uint32_t index) const
	{
		return mIndices[index];
	}

	inline Point3f GetPosition(uint32_t index) const
	{
		return mPositions[index];
	}

	inline Point2f GetTexCoord(uint32_t index) const
    {
        return mTexCoords[index];
    }

	inline Vec3f GetNormal(uint32_t index) const
	{
		return mNormals[index];
	}

	~Mesh() = default;


private:

	std::vector<Point3f> mPositions{};

	std::vector<Point2f> mTexCoords{};

	std::vector<Vec3f> mNormals{};

	std::vector<Index> mIndices{};

	// TextureNames mTextureNames;

};