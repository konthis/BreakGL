#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "game_state.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

class Window{
    private:
    GLFWwindow* mWindow;

    public:
    int init(GLuint width, GLuint height, const char* title);

    GLuint processKeyPress(GameState &gstate);

    bool shouldClose();

    void swapBuffers();

    void pollEvents();

    void kill();
};
