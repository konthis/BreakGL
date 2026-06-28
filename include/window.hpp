#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

class Window{
    private:
    GLFWwindow* mWindow;
    void processWindowInput();

    public:
    int init(GLuint width, GLuint height, const char* title);

    bool shouldClose();

    void swapBuffers();

    void pollEvents();

    void kill();
};
