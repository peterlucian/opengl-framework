#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const std::string &title);
    ~Window();

    //GLFWwindow* window_ = nullptr;

    void open();
    void close();
    bool isOpen() const;
    bool Create();
    bool ShouldClose() const;

    void SwapBuffers();
    void PollEvents();

    GLFWwindow* GetNativeWindow() const;
    void SetMouseButtonCallback(GLFWmousebuttonfun callback);
    void SetFramebufferSizeCallback(GLFWframebuffersizefun callback);
    //GLFWwindow* getWindow();
    int getWidth() const;
    int getHeight() const;
    const std::string &getTitle() const;
    void setTitle(const std::string &title);

private:
    GLFWwindow* m_Window = nullptr;

    int width_;
    int height_;
    std::string title_;
    bool is_open_;

};

#endif // WINDOW_H