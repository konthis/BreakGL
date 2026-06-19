cmake_minimum_required(VERSION 3.15.0)

project(PongGL)

add_executable(${PROJECT_NAME} src/main.cpp)

include_directories(${PROJECT_NAME} PRIVATE 
                        include,
                        )