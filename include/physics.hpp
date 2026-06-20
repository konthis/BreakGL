#pragma once
#include "objects.hpp"
#include <vector>
#include <glm/vec2.hpp>

const float GRAVITY_ACCEL   = 9.80665f;
const float dt              = 0.1f;
const u_int LIFETIME        = 20;


void apply_forces(const GameObject &object,
                std::vector<glm::vec3> &forces);


