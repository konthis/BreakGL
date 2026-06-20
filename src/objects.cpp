#include "objects.hpp"



GameObject::GameObject(const ObjectType &ot, const glm::vec2 &position, const glm::vec3 &color){
    this->ot        = ot;
    this->color     = color;
    this->position  = position;
}

Ball::Ball(const glm::vec2 &position, const glm::vec3 &color, const float velocity, const glm::vec3 &direction): 
    GameObject(ObjectType::BALL, position, color){
    this->velocity  = velocity;
    this->direction = direction;
};