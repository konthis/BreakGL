#pragma once
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define MASS_BALL 20


enum ObjectType{BALL,BRICK,PLATFORM,POWERUP};

class GameObject{
    ObjectType ot;
    glm::vec2 position;
    glm::vec3 color;
    public:
    GameObject(const ObjectType &ot, const glm::vec2 &position, const glm::vec3 &color);
    ~GameObject();


    // << is a non-member func
    friend std::ostream& operator<<(std::ostream& os, const GameObject& obj) {
        os  << "GameObject\n" 
            << "Color: " << glm::to_string(obj.color) << "\n" 
            << "Position: " << glm::to_string(obj.position) << "\n";
        return os;
    }

};

class Ball: public GameObject{
    float velocity;
    glm::vec2 direction;
    float radius;
    public:
    Ball(const glm::vec2 &position, const glm::vec3 &color, const float velocity, const glm::vec3 &direction);
    ~Ball();
    void spawn();

};