#include <iostream>
#include <chrono>
#include <string>

#include <filesystem>
namespace fs = std::filesystem;

#include "Core/Include/Window.h"
#include "Core/Include/Renderer.h"
#include "../Shaders/BlinPhongShader.h"


constexpr int WND_WIDTH  = 800;
constexpr int WND_HEIGHT = 800;

//-------------------- globals ------------------------
//-----------------------------------------------------

int main(int argc, char **argv)
{
    Window window{ WND_WIDTH, WND_HEIGHT };

    if (!window.Init()) {
        return 0;
    }

    Renderer renderer{ WND_WIDTH, WND_HEIGHT, fs::current_path().parent_path() };

    renderer.SetWindowPointer(&window);
    renderer.LoadScene();

    while(window.Run()) {

        renderer.OnUpdate();
        renderer.OnRender();      

    }


    return 0;
}




