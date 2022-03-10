#include "../Include/Texture.h"
#include "../Include/ARGB.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <memory>

Texture::Texture(const std::string& filename)
{
    LoadTextureFromString(filename);
}

void Texture::LoadTextureFromString(const std::string& filename)
{
    int x, y, n;
    if (auto pData = stbi_load(filename.c_str(), &x, &y, &n, 0))
    {
        mWidth = x;
        mHeight = y;
        mBytePerPixel = n;

        mData.resize(mWidth * mHeight * mBytePerPixel);
        for (auto i = 0; i < mWidth; i++)
        {
            for (auto j = 0; j < mHeight; j++)
            {
                auto index = j * mWidth + i;
                mData[3 * index + 0] = pData[3 * index + 0] / 255.0f;
                mData[3 * index + 1] = pData[3 * index + 1] / 255.0f;
                mData[3 * index + 2] = pData[3 * index + 2] / 255.0f;

            }
        }

        stbi_image_free(pData);
    }

}

const Color3f Texture::GetTexel(const Vec2f& texcoord) const
{
    auto uInt = static_cast<uint32_t>(texcoord.u * (mWidth - 1) + 0.5f);
    auto vInt = static_cast<uint32_t>(texcoord.v * (mHeight - 1) + 0.5f);

    assert(uInt < mWidth && vInt < mHeight);

    auto index = vInt * mWidth + uInt;

    if (mBytePerPixel == 3)
    {
        return Color3f{
             mData[3 * index + 0], 
             mData[3 * index + 1],
             mData[3 * index + 2]
        };
    }


    return Color3f{ 1.0f, 1.0f, 1.0f};
}