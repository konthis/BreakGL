#include "scenes.hpp"

void loadScene1(ECSOrganizer& ecs, Shader *ballShader, Shader *platformShader, Shader *squareShader, glm::vec2 windowSize, GLuint HUDheight){
    std::mt19937 rng(1337);
    std::uniform_real_distribution<float> col(0.f, 1.f);
    std::uniform_real_distribution<float> ballVelDirAngleRNG(glm::pi<float>()/3.0f, 5.0f * glm::pi<float>()/6.0f);

    // platform
    createPlatform(ecs,platformShader,
        glm::vec2{WINDOW_WIDTH/2.0f, PLATFORM_SIDE_SMALL/2.0f}
    );
    //

    // ball
    float ballVelDirAngle = ballVelDirAngleRNG(rng);
    createBall(ecs, ballShader,
        glm::vec2{WINDOW_WIDTH/2.0f,WINDOW_HEIGHT/7.0f},
        glm::vec2{glm::cos(ballVelDirAngle), glm::sin(ballVelDirAngle)}*BALL_ABS_SPEED
    );
    //

    // squares
    buildLayout(ecs,pattern1,squareShader);
    //

    // text
    createText(ecs, "FPS: ",COLOR_GREEN,0.5f,
        glm::vec2{0.0f, WINDOW_HEIGHT-HUD_HEIGHT+25.0f}
    );
    //


}


void loadMainMenuScene(ECSOrganizer& ecs, Shader *textShader, glm::vec2 windowSize){
    
    Entity e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 0,
        .isSelected = true
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Choose game scene",
        .color = MENU_SELECTED_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,4.0f*WINDOW_HEIGHT/5.0f}
    });

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 1,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Play",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,3.0f*WINDOW_HEIGHT/5.0f}
    });

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 2,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Settings",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,2.0f*WINDOW_HEIGHT/5.0f}
    });

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 3,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Quit",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,WINDOW_HEIGHT/5.0f}
    });

}

void buildLayout(ECSOrganizer& ecs, const std::vector<std::string>& pattern, Shader *squareShader) {
    glm::vec2 start = {20.0f,600-50.0f};    
    for (int row = 0; row < pattern.size(); row++) {
        for (int col = 0; col < pattern[row].size(); col++) {
            char c = pattern[row][col];
            if (c == '.') continue;
            else if(c == 'O'){
                glm::vec2 pos = start + glm::vec2(col * (SQUARE_SIDE+ SQUARE_GAP), -row * (SQUARE_SIDE + SQUARE_GAP)); 
                createSquare(ecs, squareShader, pos,
                    PowerUp::SUMMON_BALL
                );
            }
            else if (c == 'X'){
                glm::vec2 pos = start + glm::vec2(col * (SQUARE_SIDE+ SQUARE_GAP), -row * (SQUARE_SIDE + SQUARE_GAP)); 
                createSquare(ecs, squareShader, pos,
                    PowerUp::EMPTY
                );
            }
        }
    }
}

void loadGameOverScene(ECSOrganizer &ecs,Shader *textShader){
    Entity e = ecs.createEntity();
    ecs.addComponent<Text>(e,Text{
        .content = "GAME OVER",
        .color = MENU_TEXT_COLOR,
        .scale = 3.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,3.5f*WINDOW_HEIGHT/5.0f}
    });

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 0,
        .isSelected = true
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Play Again",
        .color = MENU_SELECTED_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,2.0f*WINDOW_HEIGHT/5.0f}
    });

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 1,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Quit",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,WINDOW_HEIGHT/5.0f}
    });
}


void loadPausedScene(ECSOrganizer &ecs,Shader *textShader){

    Entity e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 0,
        .isSelected = true
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Continue",
        .color = MENU_SELECTED_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,2.0f*WINDOW_HEIGHT/5.0f}
    });
    ecs.addComponent<PauseMenu>(e,PauseMenu{});

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 1,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Main Menu",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,WINDOW_HEIGHT/5.0f}
    });
    ecs.addComponent<PauseMenu>(e,PauseMenu{});
}