#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Math.hpp"

class Camera {


public:
    // enum class CameraMovement : unsigned int 
    // {
    //     FORWARD ,
    //     BACKWARD,
    //     LEFT,
    //     RIGHT
    // };


    Camera(
        uint32_t width,
        uint32_t height,
        Point3f origin = Vec3f{ 0.0f, 0.0f, 10.0f },
        Vec3f   target = Vec3f{ 0.0f, 0.0f, 0.0f },
        Vec3f   up = Vec3f{ 0.0f, 1.0f, 0.0f },
        float   aspectRatio = 1.0f,
        float   fov   = 45.0f,
        float   zNear = 0.01f,
        float   zFar  = 100.0f); 

    void Reset();

    void Orbit(float xOffset, float yOffset);

    void Pan(float xOffset, float yOffset);

    void Dolly(float zOffset);

    // void ProcessKeyboard(CameraMovement direction, int deltaTime);

    // void ProcessMouseScroll(int zOffset);

    void PrintInfomation();

    Mat4f GetViewMatrix() const;
    
    Mat4f GetPerspectiveMatrix() const;

public: 

    Point3f Origin;
    Point3f Target;
    Vec3f   Gaze;
    Vec3f   Up;
    Vec3f   Right;

private:    

    uint32_t mWidth;
    uint32_t mHeight;
    float mZNear;
    float mZFar;
    float mFOV;
    float mAspectRatio;

    void UpdateCameraVectors();
};



#endif