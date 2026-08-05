#include "window.h"
#include "input.h"
#include <iostream>

    extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    Window::Window(int width, int height, const std::string &title)
        : width_(width), height_(height), title_(title), is_open_(false) {}

    Window::~Window() {
        
        if (m_Window)
            glfwDestroyWindow(m_Window);
         
        glfwTerminate();
    }

    bool Window::isOpen() const {
        return is_open_;
    }

    // void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    //     {
    //         // make sure the viewport matches the new window dimensions; note that width and 
    //         // height will be significantly larger than specified on retina displays.
    //         glViewport(0, 0, width, height);
    //     }


    GLFWwindow* Window::GetNativeWindow() const
    {
        return m_Window;
    }

    bool Window::Create()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(
            width_,
            height_,
            title_.c_str(),
            nullptr,
            nullptr);

        if (!m_Window)
            return false;

        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            return false;

        return true;

    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::SetFramebufferSizeCallback(GLFWframebuffersizefun callback)
    {
        glfwSetFramebufferSizeCallback(m_Window, callback);
    }

    void Window::SetMouseButtonCallback(GLFWmousebuttonfun callback)
    {
        glfwSetMouseButtonCallback(m_Window, callback);
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

   
    