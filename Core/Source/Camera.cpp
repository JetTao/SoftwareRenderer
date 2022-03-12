#include "../Include/Camera.h"


static constexpr float KEYBOARD_SPEED          { 0.005f };
static constexpr float L_MOUSEMOVE_SENSITIVITY { 0.2f };
static constexpr float R_MOUSEMOVE_SENSITIVITY { 0.05f };


Camera::Camera(
    uint32_t width,
    uint32_t height,
    Point3f position,
    Point3f target, 
    Vec3f up,
    float fov,
    float zNear,
    float zFar)
    : mWidth(width)
    , mHeight(height)
    , Position(position)
    , Target(target)
    , Up(up)
    , mFOV(fov)
    , mZNear(zNear)
    , mZFar(zFar)
{
    UpdateCameraVectors();
}

void Camera::Reset()
{
    mZNear = 0.01f;
    mZFar  = 100.0f;
    mFOV   = 45.0f;    

    Position = Vec3f{ 0.0f, 0.0f, 5.0f };
    Target = Vec3f{ 0.0f, 0.0f, 0.0f };
    Up     = Vec3f{ 0.0f, 1.0f, 0.0f };

    UpdateCameraVectors();
}

void Camera::Orbit(float xOffset, float yOffset)
{
    float radius = VectorLength(Position);        //Spherical coordinate system
    float theta  = acos(Position.y / radius);    //ISO convention
    float phi    = atan2(Position.x , Position.z);

    float sensitivity = 1.0f / mHeight * 2 * PI;

    phi   -= xOffset * sensitivity;
    theta -= yOffset * sensitivity;
    
    theta = std::min(theta, PI - EPSILON);
    theta = std::max(theta, EPSILON);

    Position.x = radius * sin(theta) * sin(phi);
    Position.y = radius * cos(theta);
    Position.z = radius * sin(theta) * cos(phi);

    UpdateCameraVectors();

}

void Camera::Pan(float xOffset, float yOffset)
{
    float targetDistance = VectorLength(Target - Position);
    float moveDistance = targetDistance * tan(Radians(mFOV / 2));
    float sensitivity = 1.0f / mHeight * moveDistance;

    Target -= Right * (2 * xOffset * sensitivity);
    Target += Up * (2 * yOffset * sensitivity);

    UpdateCameraVectors();
}

void Camera::Dolly(float zOffset)
{
    Position += Gaze * zOffset;

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    Gaze   = Normalize(Target - Position);
    Right  = Normalize(Cross(Gaze, Up));
}

void Camera::PrintInfomation()
{
    std::cout << "Position: " << Position 
              << "Gaze: " << Gaze 
              << "Right: " << Right << std::endl;
}

Mat4f Camera::GetViewMatrix() const
{
    return LookAt(Position, Gaze, Up);
}

Mat4f Camera::GetPerspectiveMatrix() const
{
    return Perspective(mFOV, mWidth * 1.0f / mHeight, mZNear, mZFar);
}