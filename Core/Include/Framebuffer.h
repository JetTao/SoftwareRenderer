#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>
#include <cassert>

class Framebuffer {
    
public:

    Framebuffer() = default;

    Framebuffer(uint32_t width, uint32_t height)
        : mWidth(width), mHeight(height)
    {
        Allocate(mWidth, mHeight);
    }

    void Allocate(uint32_t width, uint32_t height);

    inline std::vector<uint32_t>& GetColorBuffer()
    {
        return mColorBuffer;
    }

    // inline std::vector<float> GetDepthBuffer()
    // {
    //     return mDepthBuffer;
    // }

    inline uint32_t GetWidth() const
    {
        return mWidth;
    }

    inline uint32_t GetHeight() const
    {
        return mHeight;
    }

    inline void SetColor(uint32_t x, uint32_t y, uint32_t value)
    {
        assert(x < mWidth && y < mHeight);
        mColorBuffer[mWidth * y + x] = value;
    }

    inline void SetDepth(uint32_t x, uint32_t y, float value)
    {
        assert(x < mWidth && y < mHeight);
        mDepthBuffer[mWidth * y + x] = value;
    }

    inline uint32_t GetColor(uint32_t x, uint32_t y) const
    {
        return mColorBuffer[mWidth * y + x];
    }

    inline float GetDepth(uint32_t x, uint32_t y) const
    {
        return mDepthBuffer[mWidth * y + x];
    }

    void Reset();

    void Clear();

    ~Framebuffer();

private:

    void ResetColor();

    void ResetDepth();
    
private:

    uint32_t mWidth;

    uint32_t mHeight;
    
    std::vector<uint32_t> mColorBuffer;
    
    std::vector<float> mDepthBuffer;
};

#endif //FRAME_BUFFER_H