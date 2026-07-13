cmake_minimum_required(VERSION 3.15.0)

project(BreakGL)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Freetype REQUIRED)



add_executable(${PROJECT_NAME} 
                        src/main.cpp
                        include/glad/glad.c
                        src/shader.cpp
                        src/game.cpp
                        src/window.cpp
                        src/scenes.cpp
                        src/prefabs.cpp
                        src/miniaudio.c
                        src/audio_manager.cpp
                        )


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SHADER_DIR="shaders"
    FONTS_DIR="fonts"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/audio $<TARGET_FILE_DIR:${PROJECT_NAME}>/audio
)


target_include_directories(${PROJECT_NAME} PRIVATE
                        include
                        shaders
                        include/ECS
                        PRIVATE ${FREETYPE_INCLUDE_DIRS}
                        )

target_link_libraries(${PROJECT_NAME} PRIVATE
                        glfw 
                        OpenGL::GL
                        glm::glm
                        ${FREETYPE_LIBRARIES}
)
