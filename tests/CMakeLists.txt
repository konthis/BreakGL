cmake_minimum_required(VERSION 3.15.0)

project(run_tests)

find_package(glm CONFIG REQUIRED)
add_executable(${PROJECT_NAME} 
                        ecs_test.cpp
                        # include/glad/glad.c
                        )


target_include_directories(${PROJECT_NAME} PRIVATE
                        ../include/ECS
                        )

target_link_libraries(${PROJECT_NAME} PRIVATE
                        glm::glm
)
