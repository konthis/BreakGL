#pragma once
#include "ECSorganizer.hpp"
#include "components.hpp"
#include "constants.hpp"
#include "prefabs.hpp"
#include <random>
#include <glm/gtc/constants.hpp>

const std::vector<std::string> pattern1 = {
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXX..XXXXXXXXXXXXXXXXXX",
    "XX..XXXOXXXXXXXXXXX..XXXXXXXXXXOXXX..XX",
    "XX..XXXXXXXXXXXXXXX..XXXXXXXXXXXXXX..XX",
    "XXXXXXXXXXXXXXXXXXX..XXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXX..XXXXXXXXXXXXXXXXXX",
    "XX..XXXXXXXXXXXXXXX..XXXXXXXXXXXXXX..XX",
    "XX..XXXXXXXXXXXXXXX..XXXXXXXXXXXXXX..XX",
    "XXXXXXXXXXXXXXXXXXX..XXXXXXXXXXXXXXXXXX",
    // "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
};

const std::vector<std::string> pattern2 = {
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
};

void buildLayout(ECSOrganizer& ecs, const std::vector<std::string>& pattern, Shader *squareShader);


void loadScene1(ECSOrganizer& ecs, Shader *ballShader, Shader *platformShader, Shader *squareShader, glm::vec2 windowSize, GLuint HUDheight);

void loadMainMenuScene(ECSOrganizer& ecs, Shader *textShader, glm::vec2 windowSize);

void loadGameOverScene(ECSOrganizer &ecs,Shader *textShader);

void loadPausedScene(ECSOrganizer &ecs,Shader *textShader);