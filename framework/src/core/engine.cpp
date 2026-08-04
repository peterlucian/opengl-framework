#include "engine.h"

bool Engine::Init()
{
    if (m_Initialized)
        return true;

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return false;
    }

    m_Initialized = true;
    return true;
}

void Engine::Shutdown()
{
    if (m_Initialized)
    {
        glfwTerminate();
        m_Initialized = false;
    }
}

Engine::~Engine()
{
    Shutdown();
}

