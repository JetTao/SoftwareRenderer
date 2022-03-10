#pragma once

#include "Math.hpp"
#include <vector>

class Texture
{

public:

    Texture() = default;

    explicit Texture(const std::string& filename);

    void LoadTextureFromString(const std::string& filename);

    const Color3f GetTexel(const Vec2f& texcoord) const;

    inline uint32_t GetWidth() const
    {
        return mWidth;
    }

    inline uint32_t GetHeight() const
    {
        return mHeight;
    }

    inline std::vector<float> GetData() const
    {
        return mData;
    }

private:

    uint32_t mWidth;

    uint32_t mHeight;

    uint32_t mBytePerPixel;

    // std::vector<uint8_t> mData;

    std::vector<float> mData;

};