#include "scenes.hpp"

void loadScene(ECSOrganizer& ecs, Shader *ballShader, Shader *platformShader, Shader *squareShader, Shader *backgroundShader, GameScene gameScene, Entity &platform){
    std::mt19937 rng(1337);
    std::uniform_real_distribution<float> col(0.f, 1.f);
    std::uniform_real_distribution<float> ballVelDirAngleRNG(glm::pi<float>()/3.0f, 5.0f * glm::pi<float>()/6.0f);

    // squares & background
    switch(gameScene){
        case GameScene::Scene1:{
            loadBackground(ecs,backgroundShader,BackgroundType::SCENE_1);
            buildLayout(ecs,pattern1,squareShader);
            break;
        }
        case GameScene::Scene2:{
            loadBackground(ecs,backgroundShader,BackgroundType::SCENE_2);
            buildLayout(ecs,pattern2,squareShader);
            break;
        }
        case GameScene::Scene3:{
            loadBackground(ecs,backgroundShader,BackgroundType::SCENE_3);
            buildLayout(ecs,pattern3,squareShader);
            break;
        }
    }
    //
    // platform
    platform = createPlatform(ecs,platformShader,
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



    // text
    createText(ecs, "FPS: ",COLOR_GREEN,0.5f,
        glm::vec2{WINDOW_WIDTH - 100.0f, WINDOW_HEIGHT-HUD_HEIGHT+25.0f}
    );

    createText(ecs, "TIMER: ",COLOR_GREEN,0.5f,
        glm::vec2{10.0f, WINDOW_HEIGHT-HUD_HEIGHT+25.0f}
    );

    //
}


void loadMainMenuScene(ECSOrganizer& ecs, Shader *textShader, Shader *backgroundShader){
    

    loadBackground(ecs,backgroundShader,BackgroundType::MAIN_MENU);
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
            else if(c == 'P'){
                glm::vec2 pos = start + glm::vec2(col * (SQUARE_SIDE+ SQUARE_GAP), -row * (SQUARE_SIDE + SQUARE_GAP)); 
                createSquare(ecs, squareShader, pos,
                    PowerUp::LONGER_PLATFORM
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

void loadGameOverScene(ECSOrganizer &ecs,Shader *textShader, Shader *backgroundShader){
    loadBackground(ecs,backgroundShader,BackgroundType::LOSE);
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
        .position = glm::vec2{WINDOW_WIDTH/2.0f,4.0f*WINDOW_HEIGHT/6.0f}
    });
    ecs.addComponent<PauseMenu>(e,PauseMenu{});

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 1,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Settings",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,3.0f*WINDOW_HEIGHT/6.0f}
    });
    ecs.addComponent<PauseMenu>(e,PauseMenu{});

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 2,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Main Menu",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,2.0f*WINDOW_HEIGHT/6.0f}
    });
    ecs.addComponent<PauseMenu>(e,PauseMenu{});

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
        .position = glm::vec2{WINDOW_WIDTH/2.0f,WINDOW_HEIGHT/6.0f}
    });
    ecs.addComponent<PauseMenu>(e,PauseMenu{});
}

void loadWinningScene(ECSOrganizer &ecs,Shader *textShader, Shader *backgroundShader){
    loadBackground(ecs,backgroundShader,BackgroundType::WIN);
    Entity e = ecs.createEntity();
    ecs.addComponent<Text>(e,Text{
        .content = "YOU WON!",
        .color = MENU_SELECTED_TEXT_COLOR,
        .scale = 3.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,3.0f*WINDOW_HEIGHT/5.0f}
    });
    ecs.addComponent<PauseMenu>(e,PauseMenu{});

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 0,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Main Menu",
        .color = MENU_TEXT_COLOR,
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
        .content = "Quit",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f,WINDOW_HEIGHT/5.0f}
    });
    ecs.addComponent<PauseMenu>(e,PauseMenu{});

}

void loadChooseGameSceneScene(ECSOrganizer &ecs, Shader *textShader, Shader *backgroundShader){
    loadBackground(ecs,backgroundShader,BackgroundType::MAIN_MENU);
    Entity e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 0,
        .isSelected = true
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Scene1",
        .color = MENU_SELECTED_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/4.0f, WINDOW_HEIGHT/6.0f}
    });

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 1,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Scene2",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{2.0f*WINDOW_WIDTH/4.0f,WINDOW_HEIGHT/6.0f}
    });

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 2,
        .isSelected = false
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Scene3",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{3.0f*WINDOW_WIDTH/4.0f,WINDOW_HEIGHT/6.0f}
    });
}

void loadSettingsScene(ECSOrganizer &ecs,Shader *textShader, Shader *squareShader, Shader *backgroundShader){
    loadBackground(ecs,backgroundShader,BackgroundType::MAIN_MENU);
    Entity e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 0,
        .isSelected = true
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Music Volume",
        .color = MENU_SELECTED_TEXT_COLOR,
        .scale = 1.0f,
        .rightAligned = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{2.0f*WINDOW_WIDTH/5.0f, 3.0f*WINDOW_HEIGHT/5.0f}
    });
    

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 1,
        .isSelected = true
    });
    ecs.addComponent<Text>(e,Text{
        .content = "SFX Volume",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .rightAligned = true,
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{2.0f*WINDOW_WIDTH/5.0f, 2.0f*WINDOW_HEIGHT/5.0f}
    });

    e = ecs.createEntity();
    ecs.addComponent<MenuOption>(e, MenuOption{
        .index = 2,
        .isSelected = true
    });
    ecs.addComponent<Text>(e,Text{
        .content = "Main Menu",
        .color = MENU_TEXT_COLOR,
        .scale = 1.0f,
        .centered = true
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{WINDOW_WIDTH/2.0f, 1.0f*WINDOW_HEIGHT/5.0f}
    });

    glm::vec2 startMusicBar = {WINDOW_WIDTH/2.0f, 3.0f*WINDOW_HEIGHT/5.0f};    
    glm::vec2 startSfxBar = {WINDOW_WIDTH/2.0f, 2.0f*WINDOW_HEIGHT/5.0f};    



    for(int i = 0; i < 10; i++){
        glm::vec2 pos = startMusicBar + glm::vec2(i*(SQUARE_SIDE+ SQUARE_GAP), SQUARE_SIDE + SQUARE_GAP); 
        createSquareVol(ecs, squareShader, pos, BarType::Music, i/10.0f // incremental size
        );

        pos = startSfxBar + glm::vec2(i*(SQUARE_SIDE+ SQUARE_GAP), SQUARE_SIDE + SQUARE_GAP); 
        createSquareVol(ecs, squareShader, pos, BarType::Sfx,
        i/10.0f // incremental size
        );
    }



}

void buildLayoutPreview(ECSOrganizer& ecs, const GLuint sceneIdx, Shader *squareShader){
    glm::vec2 start = {20.0f,600-50.0f};    

    const std::vector<std::string>& pattern = 
        sceneIdx == 0 ? pattern1 : 
        sceneIdx == 1 ? pattern2 : pattern3;


    for (int row = 0; row < pattern.size(); row++){
        for (int col = 0; col < pattern[row].size(); col++){
            char c = pattern[row][col];
            if (c == '.') continue;
            else if(c == 'O'){
                glm::vec2 pos = start + glm::vec2(col * (SQUARE_SIDE+ SQUARE_GAP), -row * (SQUARE_SIDE + SQUARE_GAP)); 
                createSquareHollow(ecs, squareShader, pos,
                    PowerUp::SUMMON_BALL, sceneIdx
                );
            }
            else if (c == 'X'){
                glm::vec2 pos = start + glm::vec2(col * (SQUARE_SIDE+ SQUARE_GAP), -row * (SQUARE_SIDE + SQUARE_GAP)); 
                createSquareHollow(ecs, squareShader, pos, 
                    PowerUp::EMPTY, sceneIdx
                );
            }
            else if(c == 'P'){
                glm::vec2 pos = start + glm::vec2(col * (SQUARE_SIDE+ SQUARE_GAP), -row * (SQUARE_SIDE + SQUARE_GAP)); 
                createSquareHollow(ecs, squareShader, pos,
                    PowerUp::LONGER_PLATFORM, sceneIdx
                );
            }
        }
    }
}

void loadBackground(ECSOrganizer &ecs, Shader *backgroundShader, BackgroundType bType){
    createBackground(ecs,backgroundShader, bType);
}