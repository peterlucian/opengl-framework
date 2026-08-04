#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Engine {
public:
    Engine() = default;
    ~Engine();

    bool Init();
    void Shutdown();

private:
    bool m_Initialized = false;
};

#endif // CORE_ENGINE_H
