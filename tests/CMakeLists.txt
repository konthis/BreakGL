cmake_minimum_required(VERSION 3.15.0)

project(run_tests)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)


add_executable(${PROJECT_NAME} 
                        game_test.cpp
                        ../include/glad/glad.c
                        ../src/shader.cpp
                        ../src/game.cpp
                        ../src/window.cpp
                        )


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    SHADER_DIR="${CMAKE_SOURCE_DIR}/../shaders"
)


target_include_directories(${PROJECT_NAME} PRIVATE
                        ../include
                        ../shaders
                        ../include/ECS
                        )

target_link_libraries(${PROJECT_NAME} PRIVATE
                        glfw 
                        OpenGL::GL
                        glm::glm
)
