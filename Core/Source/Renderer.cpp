#include "../Include/Renderer.h"
#include "../Include/ARGB.h"
#include "../Include/Window.h"

#include "../../Shaders/BlinPhongShader.h"

#include <memory>
#include <array>


void Renderer::LoadScene(void)
{
    
    fs::path crabPath = mProjectPath / "Scenes/crab/crab.obj";
    auto pCrabModel = std::make_shared<Model>(crabPath);

    mModels["crab"] = pCrabModel;
}


void Renderer::OnUpdate()
{
    mTimer.Tick();

    auto deltaTime = mTimer.GetDeltaSeconds();

    static uint64_t frameCount = 0;

    static double totalTime = 0.0;

    totalTime += deltaTime;

    frameCount++;

    if (totalTime > 1.0)
    {
        double fps = frameCount / totalTime;

        wchar_t buffer[256];
        swprintf_s(buffer, L"SoftwareRenderer   FPS: %.f  %.fms\n",
         fps, mTimer.GetDeltaMilliseconds());
        
        auto hwnd = mpWindow->GetWindowHandle();

        if (hwnd)
        {
            SetWindowTextW(hwnd, buffer);
        }

        frameCount = 0;
        totalTime = 0.0;
    }

    OnKeyboardInput();
    OnMouseMoveInput();
    mpWindow->UpdateWindowColor();

}

void Renderer::OnRender()
{
    Mat4f model     { Mat4f::GetIdentity() };        
    Mat4f projection{ mCamera.GetPerspectiveMatrix() };
    Mat4f view      { mCamera.GetViewMatrix() }; 

    static float theta = 0;
    

    Point3f lightPosition{ 0, 0.0f, 100.0f };
    model = Rotate(model, theta, Vec3f{ 0.0f, 1.0f, 0.0f });
    lightPosition = model * Point4f{ lightPosition, 1.0f };

    theta += mTimer.GetDeltaMilliseconds() * 0.1f ;

    model = Mat4f::GetIdentity();

    BlinPhongShader::PointLight light{
        Point3f{ lightPosition },
        Color3f{ 0.2f, 0.2f, 0.2f },
        Color3f{ 0.5f, 0.5f, 0.5f },
        Color3f{ 1.0f, 1.0f, 1.0f }
    };

    auto pBackPackModel = mModels["crab"];

    auto pBackPackShader = std::make_shared<BlinPhongShader>();

    pBackPackShader->Model = model;
    pBackPackShader->View = view;
    pBackPackShader->Projection = projection;
    pBackPackShader->Light = light;
    pBackPackShader->ViewPosition = mCamera.Position;

    pBackPackShader->pDiffuseMap = pBackPackModel->GetDiffuse();
    pBackPackShader->pNormalMap = pBackPackModel->GetNormal();
    pBackPackShader->pSpecularMap = pBackPackModel->GetSpecular();

    DrawModel(pBackPackModel, pBackPackShader);
 
    mpWindow->SwapColorBuffer(mFramebuffer.GetColorBuffer());
    mFramebuffer.Reset();

    // exit(0);
}



void Renderer::DrawModel(
    std::shared_ptr<Model> pModel,
    std::shared_ptr<BaseShader> pShader)
{

    auto pMesh = pModel->GetMesh();

    for (int i = 0; i < pMesh->NumIndices(); i += 3)
    {
        std::array<Vertex, 3> triangle{};
        for (int j = 0; j < 3; j++)
        {
            auto positionIndex = pMesh->GetIndex(i + j).PositionIndex;
            auto texCoordIndex = pMesh->GetIndex(i + j).TexCoordIndex;
            auto normalIndex = pMesh->GetIndex(i + j).NormalIndex;

            if (positionIndex < pMesh->NumPositions())
            {
                triangle[j].Position = pMesh->GetPosition(positionIndex);
            }
            if (texCoordIndex < pMesh->NumTexCoords())
            {
                triangle[j].TexCoord = pMesh->GetTexCoord(texCoordIndex);
            }
            if (normalIndex < pMesh->NumNormals())
            {
                triangle[j].Normal   = pMesh->GetNormal(normalIndex);
            }
        }

            if (CullBackface(
                triangle[0].Position,
                triangle[1].Position,
                triangle[2].Position))
            {
                continue;
            }

        for (auto &&vertex : triangle) 
        {
            pShader->VertexShader(vertex);
            vertex.RHW = 1.0f / vertex.Position.w;
        }

        auto triangles = Clip(triangle);

        for (auto &&triangle : triangles) 
        {
            for (auto&& vertex : triangle)
            {
                vertex.Position = Viewport(mWidth - 1, mHeight - 1)
                    * vertex.Position.Homogenize();
            }
            
            DrawTriangle(pShader, triangle);
        }

    }

}

void Renderer::DrawPixel(uint32_t x, uint32_t y, const ARGB &color)
{
    mFramebuffer.SetColor(x, y, color.argb);
}

void Renderer::DrawLine(Point2f t0, Point2f point1, const ARGB &color)
{  
    bool steep = false;
    if (std::abs(t0.x - point1.x) < std::abs(t0.y - point1.y)) {
        std::swap(t0.x, t0.y);
        std::swap(point1.x, point1.y);
        steep = true;
    }
    if (t0.x > point1.x) {
        std::swap(t0.x, point1.x);
        std::swap(t0.y, point1.y);
    }
    int dx = point1.x - t0.x;
    int dy = point1.y - t0.y;
    float derror = std::abs(dy) * 2;
    float error = 0;
    int y = t0.y;
    for (int x = int(t0.x + 0.5f); x <= point1.x; x++) {
        if (steep) {
            DrawPixel(y, x, color);
        }
        else {
            DrawPixel(x, y, color);
        }
        error += derror;
        if (error > dx) {
            y += (point1.y > t0.y ? 1 : -1);
            error -= dx * 2;
        }
    }
}

Point3f Renderer::GetBarycentricCoord(
    const Point3f &A,
    const Point3f &B,
    const Point3f &C,
    const Point2f &P)
{
    float barycX = -1.0f; 
    float barycY = -1.0f;
    float barycZ = -1.0f;
    
    float denom = 1.0f / ((A.y - C.y) * (B.x - C.x) + (B.y - C.y) * (C.x - A.x));
    barycX = ((P.y - C.y) * (B.x - C.x) + (B.y - C.y) * (C.x - P.x)) * denom;
    barycY = ((P.y - A.y) * (C.x - A.x) + (C.y - A.y) * (A.x - P.x)) * denom;
    // float barycZ = ((P.y - B.y) * (A.x - B.x) + (A.y - B.y) * (B.x - P.x)) * denom;
    barycZ = 1.0f - barycX - barycY;

    return Vec3f(barycX, barycY, barycZ);
}

void Renderer::DrawTriangle(
    std::shared_ptr<BaseShader> pShader,
    std::array<Vertex, 3>& triangle)
{
    float maxX, maxY;
    float minX, minY;

    maxX = minX = triangle[0].Position.x;
    maxY = minY = triangle[0].Position.y;
    for (int i = 1; i <= 2; i++) {
        maxX = std::max<float>(maxX, triangle[i].Position.x);
        minX = std::min<float>(minX, triangle[i].Position.x);
        maxY = std::max<float>(maxY, triangle[i].Position.y);
        minY = std::min<float>(minY, triangle[i].Position.y);
    }
   
    int bottomX = static_cast<int>(minX + 0.5f);
    int topX    = static_cast<int>(maxX + 0.5f);
    int bottomY = static_cast<int>(minY + 0.5f);
    int topY    = static_cast<int>(maxY + 0.5f);

    assert(topX >= 0 && topX < mWidth); 
    assert(topY >= 0 && topY < mHeight); 
    assert(bottomX >= 0 && bottomX < mWidth);
    assert(bottomY >= 0 && bottomY < mHeight);     

    for (int x = bottomX; x <= topX; x++) 
    {
        for (int y = bottomY; y <= topY; y++) 
        {

            Point2f point{ static_cast<float>(x), static_cast<float>(y) };
            auto barycentricCoord = GetBarycentricCoord(
                Point3f{ triangle[0].Position },
                Point3f{ triangle[1].Position },
                Point3f{ triangle[2].Position }, 
                point);

            if (barycentricCoord.x > 0 && barycentricCoord.y > 0 && barycentricCoord.z > 0) 
            {
                Color4f color{ 0.0f, 0.0f, 0.0f, 0.0f };
                Point2f texCoord{ 0.0f, 0.0f };
                Vec3f normal{ 0.0f, 0.0f, 0.0f };
                float reciprocalDepth = 0.0f;
                for (int i = 0; i < 3; i++)
                {
                    reciprocalDepth += barycentricCoord[i] * triangle[i].RHW;    // 1/z = 1/z1*s + 1/z2*(1-s)
                    color += barycentricCoord[i] * triangle[i].Color * triangle[i].RHW;
                    texCoord += barycentricCoord[i] * triangle[i].TexCoord * triangle[i].RHW;
                    normal += barycentricCoord[i] * triangle[i].Normal * triangle[i].RHW;

                }

                color /= reciprocalDepth;
                texCoord /= reciprocalDepth;
                normal /= reciprocalDepth;

                float depth =  1.0f / reciprocalDepth;

                if (depth < mFramebuffer.GetDepth(x, y)) //NDC坐标系中摄像机指向+z
                {               

                    mFramebuffer.SetDepth(x, y, depth);

                    Fragment fragment{
                        Point2i{ x, y },
                        color,
                        texCoord,
                        normal,
                        depth
                    };

                    pShader->FragmentShader(fragment);
                    ARGB argbColor = Color4fToARGB(fragment.Color);
                    DrawPixel(x, y, argbColor);
                }
            }
        }
    }

}

void Renderer::DrawWireFrameTriangle(
    const Point3f& point0,
    const Point3f& point1,
    const Point3f& point2,
    const ARGB& color)
{
    DrawLine(Point2f{ point0.x, point0.y }, Point2f{ point1.x, point1.y }, color);
    DrawLine(Point2f{ point1.x, point1.y }, Point2f{ point2.x, point2.y }, color);
    DrawLine(Point2f{ point2.x, point2.y }, Point2f{ point0.x, point0.y }, color);
}

bool Renderer::CullBackface(
    const Point4f& point0,
    const Point4f& point1,
    const Point4f& point2)
{
    Vec3f vector0{ point1 - point0 };
    Vec3f vector1{ point2 - point1 };
    Vec3f normalVector{ Normalize( Cross(vector0, vector1) ) };

    float degrees = Dot(normalVector, -mCamera.Gaze);

    return degrees < 0;
}


std::vector<std::array<Vertex, 3>> 
Renderer::Clip(const std::array<Vertex, 3>& triangle)
{

    std::vector<Vertex> clippingVertices{
        triangle[0],
        triangle[1],
        triangle[2]
    };
    std::vector<Vertex> clippedVertices;

    if (ClipSpaceCull(clippingVertices[0].Position,
                      clippingVertices[1].Position, 
                      clippingVertices[2].Position))
    {
        
        for (uint32_t CLIP_PLANE = EPSILON_PLANE; CLIP_PLANE <= FAR_PLANE; CLIP_PLANE++) 
        {
            ClipPolygonAgainstPlane(clippingVertices, clippedVertices, CLIP_PLANE);

            std::swap(clippingVertices, clippedVertices);

            if (CLIP_PLANE != FAR_PLANE) 
            {
                clippedVertices.clear();
            }
        }
    }

    std::vector<std::array<Vertex, 3>> triangles;

    const int numClippedTriangles = clippedVertices.size() - 2;
    for (auto i = 0; i < numClippedTriangles; i++) 
    {
        
        std::array<Vertex, 3> triangle = {
             clippedVertices[0],
             clippedVertices[i + 1],
             clippedVertices[i + 2]
        };

        triangles.push_back(triangle);
    }

    return triangles;
}

bool Renderer::ClipSpaceCull(
    const Point4f &point1,
    const Point4f &point2,
    const Point4f &point3)
{
    if (point1.w < EPSILON && point2.w < EPSILON && point3.w < EPSILON) 
        return false;
    if (std::abs(point1.x) > point1.w && std::abs(point2.x) > point2.w && std::abs(point3.x) > point3.w)
        return false;
    if (std::abs(point1.y) > point1.w && std::abs(point2.y) > point2.w && std::abs(point3.y) > point3.w)
        return false;
    if (std::abs(point1.z) > point1.w && std::abs(point2.z) > point2.w && std::abs(point3.z) > point3.w)
        return false;

    return true;
}

bool Renderer::IsInsidePlane(Point4f &clipPoint, uint32_t PLANE)
{
    switch (PLANE) {
        case EPSILON_PLANE: 
            return EPSILON < clipPoint.w;
        case TOP_PLANE:
            return clipPoint.y  < clipPoint.w;
        case LEFT_PLANE:
            return -clipPoint.x < clipPoint.w;
        case BOTTOM_PLANE:
            return -clipPoint.y < clipPoint.w;
        case RIGHT_PLANE:
            return clipPoint.x  < clipPoint.w;
        case NEAR_PLANE:
            return -clipPoint.z < clipPoint.w;
        case FAR_PLANE:
            return clipPoint.z  < clipPoint.w;
        default:
            return false;
        break;
    }
}

float Renderer::GetIntersectionFactor(
    Point4f &previousPoint,
    Point4f &currentPoint,
    uint32_t PLANE)
{
    switch (PLANE) 
    {
        case EPSILON_PLANE:
            return (EPSILON - previousPoint.w) / (currentPoint.w - previousPoint.w);
        case TOP_PLANE:
            return (previousPoint.y - previousPoint.w) / 
                   ((previousPoint.y - previousPoint.w) - (currentPoint.y - currentPoint.w));
        case LEFT_PLANE: 
            return (previousPoint.x + previousPoint.w) / 
                   ((previousPoint.x + previousPoint.w) - (currentPoint.x + currentPoint.w));
        case BOTTOM_PLANE:
            return (previousPoint.y + previousPoint.w) /
                   ((previousPoint.y + previousPoint.w) - (currentPoint.y + currentPoint.w));
        case RIGHT_PLANE:
            return (previousPoint.x - previousPoint.w) /
                   ((previousPoint.x - previousPoint.w) - (currentPoint.x - currentPoint.w));
        case NEAR_PLANE:
            return (previousPoint.z + previousPoint.w) /
                   ((previousPoint.z + previousPoint.w) - (currentPoint.z + currentPoint.w));
        case FAR_PLANE:
            return (previousPoint.z - previousPoint.w) /
                   ((previousPoint.z - previousPoint.w) - (currentPoint.z - currentPoint.w));
        default:

            return 0.0f;
    }
}

void Renderer::ClipPolygonAgainstPlane(
    std::vector<Vertex> &inputVertices,
    std::vector<Vertex> &outputVertices,
    uint32_t PLANE)
{

    Vertex previousVertex;
    Vertex currentVertex;
    
    bool previousDot;
    bool currentDot;

    float intersectionFactor;
    Vertex intersectionPoint;
    
    const int NUM_VERTICES = inputVertices.size();

    for (size_t i = 0; i < NUM_VERTICES; i++) 
    {
        previousVertex = inputVertices[i];
        currentVertex  = inputVertices[(i + 1) % NUM_VERTICES];

        previousDot = IsInsidePlane(previousVertex.Position, PLANE);
        currentDot = IsInsidePlane(currentVertex.Position, PLANE);

        if (previousDot != currentDot) 
        {
            intersectionFactor = GetIntersectionFactor(previousVertex.Position,
                                                       currentVertex.Position, 
                                                       PLANE);

            intersectionPoint.Position = (currentVertex.Position - previousVertex.Position) * 
                                          intersectionFactor + previousVertex.Position;
                                         
            intersectionPoint.TexCoord = (currentVertex.TexCoord - previousVertex.TexCoord) *
                                          intersectionFactor + previousVertex.TexCoord;
                                         
            intersectionPoint.Normal = (currentVertex.Normal - previousVertex.Normal) *
                                        intersectionFactor + previousVertex.Normal;
                                          
            intersectionPoint.RHW = 1.0f / intersectionPoint.Position.w; 

            outputVertices.push_back(intersectionPoint);
        }
        if (currentDot) 
        {
            outputVertices.push_back(currentVertex);  
        }
    }
}


void Renderer::OnKeyboardInput()
{
    float deltaTime = mTimer.GetDeltaMilliseconds();

    if (mpWindow->GetKeyState('R'))
    {
        mCamera.Reset();
        return;
    }
    if (mpWindow->GetKeyState('W'))
    {
        mCamera.Dolly(deltaTime / 100.0f);
    }
    if (mpWindow->GetKeyState('S'))
    {
        mCamera.Dolly(-deltaTime / 100.0f);
    }
    if (mpWindow->GetKeyState('A'))
    {
        mCamera.Pan(-deltaTime, 0.0f);
    }
    if (mpWindow->GetKeyState('D'))
    {
        mCamera.Pan(deltaTime, 0.0f);
    }
    if (mpWindow->GetKeyState(VK_UP))
    {
        mCamera.Orbit(0.0f, -deltaTime / 8.0f);
    }
    if (mpWindow->GetKeyState(VK_DOWN))
    {
        mCamera.Orbit(0.0f, deltaTime / 8.0f);
    }
    if (mpWindow->GetKeyState(VK_LEFT))
    {
        mCamera.Orbit(-deltaTime / 8.0f, 0.0f);
    }
    if (mpWindow->GetKeyState(VK_RIGHT))
    {
        mCamera.Orbit(deltaTime / 8.0f, 0.0f);
    }
}

void Renderer::OnMouseMoveInput()
{
    static float lastX;
    static float lastY;
    float xOffset = (mpWindow->GetMousePosX() - lastX);
    float yOffset = (mpWindow->GetMousePosY() - lastY);
    if (mpWindow->GetButtonState(VK_LBUTTON)) {
        mCamera.Orbit(xOffset, yOffset);
    }
    if (mpWindow->GetButtonState(VK_RBUTTON)) {
        mCamera.Pan(xOffset, yOffset);
    }
    mCamera.Dolly(mpWindow->GetDeltaZ());
    mpWindow->SetDeltaZ(0);
    lastX = mpWindow->GetMousePosX();
    lastY = mpWindow->GetMousePosY();
}