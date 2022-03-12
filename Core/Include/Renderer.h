#pragma once

#include "Math.hpp"
#include "Camera.h"
#include "Framebuffer.h"
#include "HighResolutionClock.h"
#include "Model.h"
#include "Vertex.h"
#include "../../Shaders/BaseShader.h"

#include <vector>
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;


class ARGB;
class Window;


class Renderer
{

public:

    Renderer(uint32_t width, uint32_t height, const fs::path& path)
        : mWidth(width)
        , mHeight(height)
        , mFramebuffer(mWidth, mHeight)
        , mCamera(mWidth, mHeight)
        , mProjectPath(path) {}


    void SetWindowPointer(Window* pWindow)
    {
        mpWindow = pWindow;
    }

    void LoadScene();

    void SetCamera();

    void OnUpdate();

    void OnRender();

private:

void DrawModel(
    std::shared_ptr<Model> pModel,
    std::shared_ptr<BaseShader> pShader);

void DrawPixel(uint32_t x, uint32_t y, const ARGB &color);

void DrawLine(Point2f point0, Point2f point1, const ARGB &color);

void DrawTriangle(
    std::shared_ptr<BaseShader> pShader,
    std::array<Vertex, 3>& triangle);

void DrawWireFrameTriangle(
    const Point3f& point0,
    const Point3f& point1,
    const Point3f& point2,
    const ARGB& color);

Vec3f GetBarycentricCoord(
    const Point3f &A,
    const Point3f &B,
    const Point3f &C,
    const Point2f &P);

bool CullBackface(
    const Point4f& point0,
    const Point4f& point1,
    const Point4f& point2);

std::vector<std::array<Vertex, 3>> Clip(
    const std::array<Vertex, 3>& triangle
);

bool ClipSpaceCull(
    const Point4f &point1,
    const Point4f &pointt2,
    const Point4f &point3);

bool IsInsidePlane(Point4f &clipPoint, uint32_t PLANE);

float GetIntersectionFactor(
    Point4f &previousPoint, 
    Point4f &currentPoint, 
    uint32_t PLANE);

void ClipPolygonAgainstPlane(
    std::vector<Vertex> &inputVertices,
    std::vector<Vertex> &outputVertices,
    uint32_t PLANE);

void OnKeyboardInput();

void OnMouseMoveInput();

private:

    enum ClipPlane : uint32_t {
        EPSILON_PLANE,
        TOP_PLANE,
        LEFT_PLANE,
        BOTTOM_PLANE,
        RIGHT_PLANE,
        NEAR_PLANE,
        FAR_PLANE
    };

    uint32_t mWidth;

    uint32_t mHeight;

    Framebuffer mFramebuffer;

    Camera mCamera;

    fs::path mProjectPath;

    Window* mpWindow;

    HighResolutionClock mTimer;

    std::unordered_map<std::string, std::shared_ptr<Model>> mModels;

};

