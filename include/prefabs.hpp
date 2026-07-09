#pragma once
#include "constants.hpp"
#include "components.hpp"
#include "ECSorganizer.hpp"

Entity createSquare(ECSOrganizer& ecs, Shader* squareShader, glm::vec2 pos, PowerUp power);

Entity createBall(ECSOrganizer& ecs, Shader* ballShader, glm::vec2 pos, glm::vec2 vel);

Entity createPlatform(ECSOrganizer& ecs, Shader* platformShader, glm::vec2 pos);

Entity createText(ECSOrganizer& ecs, const std::string content, glm::vec4 color, float scale, glm::vec2 pos);

Entity createSquareHollow(ECSOrganizer& ecs, Shader* squareShader, glm::vec2 pos, PowerUp power, const GLuint sceneIdx);

