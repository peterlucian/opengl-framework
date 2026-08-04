#include "window.h"
#include <iostream>

    extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    window::window(int width, int height, const std::string &title)
        : width_(width), height_(height), title_(title), is_open_(false) {}

    bool window::isOpen() const {
        return is_open_;
    }

    void    framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            // make sure the viewport matches the new window dimensions; note that width and 
            // height will be significantly larger than specified on retina displays.
            glViewport(0, 0, width, height);
        }



    GLFWwindow* window::create()
    {
        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW\n";
            return nullptr;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window_ = glfwCreateWindow(
            width_,
            height_,
            title_.c_str(),
            nullptr,
            nullptr
        );

        if (!window_)
        {
            std::cout << "Failed to create GLFW window\n";
            glfwTerminate();
            return nullptr;
        }

        glfwMakeContextCurrent(window_);

        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
        glfwSetMouseButtonCallback(window_, mouse_button_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD\n";
            return nullptr;
        }

        return window_;
    }




    // GLFWwindow* window::getWindow() {
    //     if (window_ == nullptr) {
    //         std::cout << "Failed to create GLFW window" << std::endl;
    //         glfwTerminate();
    //     } else {
    //         return window_;
    //     }
    // }