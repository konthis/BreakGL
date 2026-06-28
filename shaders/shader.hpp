#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static std::string readFile(const char* path);

class Shader{
    private:
    public:
    	GLuint mID;
    	Shader(const char* vertexFile, const char* fragmentFile);
    	void activate();
    	void remove();
};