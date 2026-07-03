#pragma once
#include "ECSorganizer.hpp"
#include "components.hpp"
#include <random>

void generateSquarePositions(float sideSize, std::vector<Position> );

void loadSceneBalls(ECSOrganizer& ecs, Shader *ballShader);
void loadSceneBallsAndPlatform(ECSOrganizer& ecs, Shader *ballShader, Shader *platformShader, glm::vec2 windowSize);