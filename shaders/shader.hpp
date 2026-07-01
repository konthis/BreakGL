#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static std::string readFile(const char* path);

class Shader{
    private:
		// to save uniform variables, like color, time etc
		std::unordered_map<std::string, GLint> mUniformCache;
    public:
    	GLuint mID;
    	Shader(const char* vertexFile, const char* fragmentFile);
		GLint setUniformLocation(const std::string& name);
		GLint getUniformLocation(const std::string& name);

		template<typename T>
		void setUniform(const char* name, T value) {
    		GLint loc = getUniformLocation(name);
    		if constexpr (std::is_same_v<T, float>)
    		    glUniform1f(loc, value);
    		// else if constexpr (std::is_same_v<T, int>)
    		//     glUniform1i(loc, value);
    		// else if constexpr (std::is_same_v<T, glm::vec2>)
    		//     glUniform2f(loc, value.x, value.y);
    		// else if constexpr (std::is_same_v<T, glm::vec3>)
    		    // glUniform3f(loc, value.x, value.y, value.z);
    		else if constexpr (std::is_same_v<T, glm::vec4>)
    		    glUniform4f(loc, value.r, value.g, value.b, value.a);
    		else if constexpr (std::is_same_v<T, glm::mat4>)
    		    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
		}

    	void activate();
    	void remove();
};