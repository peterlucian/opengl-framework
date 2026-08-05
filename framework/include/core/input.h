#pragma once

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input
{
public:
    static void Init(GLFWwindow* window);
    static bool IsKeyPressed(int key);
    static bool IsMouseButtonPressed(int button);

    static glm::vec2 GetMousePosition();

private:
    friend class Window;

    static void KeyCallback(GLFWwindow*, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow*, int button, int action, int mods);
    static void CursorPositionCallback(GLFWwindow*, double x, double y);

private:
    static bool s_Keys[512];
    static bool s_MouseButtons[8];

    static float s_MouseX;
    static float s_MouseY;
};