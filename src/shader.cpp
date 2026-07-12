#include "shader.hpp"

static std::string readFile(const char* path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << "\n";
        return "";
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLint Shader::setUniformLocation(const std::string& name){
    if (mUniformCache.find(name) == mUniformCache.end())
        mUniformCache[name] = glGetUniformLocation(mID, name.c_str());
    return -1;
}

GLint Shader::getUniformLocation(const std::string& name) {
    return mUniformCache[name];
}


Shader::Shader(const char* vertPath, const char* fragPath)
{

    std::string vertSrc = readFile(vertPath);
    std::string fragSrc = readFile(fragPath);
    const char* vCode = vertSrc.c_str();
    const char* fCode = fragSrc.c_str();

    // VERT shader
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vCode, nullptr);
    glCompileShader(vert);
    GLint ok;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(vert, 512, nullptr, log);
        std::cerr << "Vertex shader error:\n" << log << "\n";
    }

    // Frag shader
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fCode, nullptr);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(frag, 512, nullptr, log);
        std::cerr << "Fragment shader error:\n" << log << "\n";
    }

    mID = glCreateProgram();
    glAttachShader(mID, vert);
    glAttachShader(mID, frag);
    glLinkProgram(mID);
    glGetProgramiv(mID, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(mID, 512, nullptr, log);
        std::cerr << "Shader link error:\n" << log << "\n";
    }

    // UNIFORM INITIALIZATIONS
    setUniformLocation("u_model");
    setUniformLocation("u_projection");
    setUniformLocation("u_platformBigSide");
    setUniformLocation("u_color");
    setUniformLocation("u_time");
    setUniformLocation("u_textColor");
    setUniformLocation("u_resolution");
    //

    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::activate(){
    glUseProgram(mID);
}

void Shader::remove(){
    glDeleteProgram(mID);
}