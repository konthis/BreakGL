#include "window.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLuint Window::processKeyPress()
{
    // ONLY HANDLES PRESSES FOR NOW
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(mWindow, true);
        return GLFW_KEY_ESCAPE;
    }
    if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_PRESS){
        return GLFW_KEY_UP;
    }
    if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_PRESS){
        return GLFW_KEY_DOWN;
    }
    if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS){
        return GLFW_KEY_LEFT;
    }
    if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS){
        return GLFW_KEY_RIGHT;
    }
    return GLFW_KEY_UNKNOWN;
}

int Window::init(GLuint width, GLuint height, const char* title){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    mWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (mWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(mWindow);
    // NO RESIZING (YET)
    // glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return 0;
}

bool Window::shouldClose(){
    return glfwWindowShouldClose(mWindow);
}

void Window::swapBuffers(){
    glfwSwapBuffers(mWindow);
}

void Window::pollEvents(){
    glfwPollEvents();
    // processKeyPress();
}


void Window::kill(){
    glfwTerminate();
}