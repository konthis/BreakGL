#pragma once
#include "ECSorganizer.hpp"
#include "components.hpp"
#include <random>
#include <glm/gtc/constants.hpp>

struct BlockLayout {
    glm::vec2 start;    
    glm::vec2 size;     
    glm::vec2 gap;     
    GLuint cols;
    GLuint rows;
    GLuint current = 0;   
    GLuint max;   

    bool hasNext() const { return current < max-1;}

    glm::vec2 next() {
        int col = current % cols;
        int row = current / cols;
        current++;
        return start + glm::vec2(col * (size.x + gap.x), -row * (size.y + gap.y));
        

    }
};


void loadSceneBalls(ECSOrganizer& ecs, Shader *ballShader);
void loadSceneBallsAndPlatform(ECSOrganizer& ecs, Shader *ballShader, Shader *platformShader, glm::vec2 windowSize);
void loadSceneBallPlatformSquares(ECSOrganizer& ecs, Shader *ballShader, Shader *platformShader, Shader *squareShader, glm::vec2 windowSize, GLuint HUDheight);
void loadTextScene(ECSOrganizer& ecs, Shader *textShader, glm::vec2 windowSize);