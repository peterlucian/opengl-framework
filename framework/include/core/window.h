#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <glad.h>
#include <GLFW/glfw3.h>

class window {
public:
    window(int width, int height, const std::string &title);
    ~window();

    //GLFWwindow* window_ = nullptr;

    void open();
    void close();
    bool isOpen() const;
    GLFWwindow* create();
    //GLFWwindow* getWindow();
    int getWidth() const;
    int getHeight() const;
    const std::string &getTitle() const;
    void setTitle(const std::string &title);

private:
    int width_;
    int height_;
    std::string title_;
    bool is_open_;

};

#endif // WINDOW_H