#include "../Include/Framebuffer.h"

void Framebuffer::Allocate(uint32_t width, uint32_t height)
{
    mWidth = width;
    mHeight = height;

    mColorBuffer.resize(mWidth * mHeight);
    mDepthBuffer.resize(mWidth * mHeight);

    std::fill(mColorBuffer.begin(), mColorBuffer.end(), 0);
    std::fill(mDepthBuffer.begin(), mDepthBuffer.end(),
        std::numeric_limits<float>::max());
}

void Framebuffer::ResetColor()
{
    std::fill(mColorBuffer.begin(), mColorBuffer.end(), 0);
}
void Framebuffer::ResetDepth()
{
    std::fill(mDepthBuffer.begin(), mDepthBuffer.end(),
        std::numeric_limits<float>::max());
}

void Framebuffer::Clear()
{
    mColorBuffer.clear();
    mDepthBuffer.clear();

    mWidth = 0;
    mHeight = 0;
}

void Framebuffer::Reset()
{
    ResetColor();
    ResetDepth();
}

Framebuffer::~Framebuffer()
{
    Clear();
}