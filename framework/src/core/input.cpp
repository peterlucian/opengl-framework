#include "Input.h"

bool Input::s_Keys[512] = {};
bool Input::s_MouseButtons[8] = {};

float Input::s_MouseX = 0.0f;
float Input::s_MouseY = 0.0f;

bool Input::IsKeyPressed(int key)
{
    return s_Keys[key];
}

bool Input::IsMouseButtonPressed(int button)
{
    return s_MouseButtons[button];
}

glm::vec2 Input::GetMousePosition()
{
    return { s_MouseX, s_MouseY };
}

void Input::Init(GLFWwindow* window)
{
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
}

void Input::KeyCallback(GLFWwindow*, int key, int, int action, int)
{
    if (key >= 0 && key < 512)
        s_Keys[key] = action != GLFW_RELEASE;
}

void Input::MouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    if (button >= 0 && button < 8)
        s_MouseButtons[button] = action != GLFW_RELEASE;
}

void Input::CursorPositionCallback(GLFWwindow*, double x, double y)
{
    s_MouseX = static_cast<float>(x);
    s_MouseY = static_cast<float>(y);
}
