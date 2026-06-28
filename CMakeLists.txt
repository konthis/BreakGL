cmake_minimum_required(VERSION 3.15.0)

project(BreakGL)

add_executable(${PROJECT_NAME} 
                        src/main.cpp
                        include/glad/glad.c
                        )

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)


target_include_directories(${PROJECT_NAME} PRIVATE
                        include
                        )

target_link_libraries(${PROJECT_NAME} PRIVATE
                        glfw 
                        OpenGL::GL
                        glm::glm
)