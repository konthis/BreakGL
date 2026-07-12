#pragma once
#include "ECSorganizer.hpp"
#include "components.hpp"
#include "constants.hpp"
#include "prefabs.hpp"
#include <random>
#include <glm/gtc/constants.hpp>

const std::vector<std::string> pattern1 = {
    "XXXXXXXXXXXXXXXXPXXXXXXPXXXXXXXXXXXXXXX",
    "XX..XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX..X",
    "XXXXOXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXOXXX",
    "XXXXXXXXXXXXXXXXXP....PXXXXXXXXXXXXXXXX",
    "XXXX...XXXXXXXXXXXXXXXXXXXXXXXXXX...XXX",
    "XXXXXXXXOXXXXXXXXXXXXXXXXXXXXXXOXXXXXXX",
    "XXX....PXXXXXXXXXXXXXXXXXXXXXXXXP....XX",
    "..XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.",
    "XOXXXXXXXXXXXXXXXX....XXXXXXXXXXXXXXXXO",
    "XXXX..XXXXXXXX..XXXXXXXX..XXXXXXXX..XXX",
};

const std::vector<std::string> pattern2 = {
    "XXXXXXX.XXXXXXX.XXXXXXX.XXXXXXX.XXXXXXX",
    "XXXPXXX.XXXPXXX.XXXPXXX.XXXXXXX.XXXPXXX",
    "XXXOXXX.XXXOXXX.XXXOXXX.XXXOXXX.XXXOXXX",
    "XXXXXXX.XXXXXXX.XXXXXXX.XXXXXXX.XXXXXXX",
    "XXXPXXX.XXXPXXX.XXXPXXX.XXXPXXX.XXXPXXX",
    "XOXOXXX.XOXOXXX.XOXOXXX.XOXOXXX.XOXOXXX",
    "XXXXXXX.XXXXXXX.XXXXXXX.XXXXXXX.XXXXXXX",
    "XXXXXOX.XXXXXOX.XXXXXOX.XXXXXOX.XXXXXOX",
    "lXXXXX...XXXXX...XXXXX...XXXXX...XXXXXX",
};

const std::vector<std::string> pattern3 = {
    "X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X",
    ".X.X.X.P.X.X.X.X.X.X.X.X.X.X.X.P.X.X.X",
    "X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X",
    ".X.X.X.X.X.X.X.X.X.P.X.X.X.X.X.X.X.X.X",
    "O.O.O.O.O.O.O.O.O.O.O.O.O.O.O.O.O.O.O.O",
    ".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X",
    "X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X",
    ".P.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.P.",
    "X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X",

};






void buildLayout(ECSOrganizer& ecs, const std::vector<std::string>& pattern, Shader *squareShader);

// for choose scene preview
void buildLayoutPreview(ECSOrganizer& ecs,const GLuint sceneIdx, Shader *squareShader);

void loadScene(ECSOrganizer& ecs, Shader *ballShader, Shader *platformShader, Shader *squareShader, Shader *backgroundShader, GameScene gameScene, Entity &platform);

void loadMainMenuScene(ECSOrganizer& ecs, Shader *textShader, Shader *backgroundShader);

void loadGameOverScene(ECSOrganizer &ecs,Shader *textShader, Shader *backgroundShader);

void loadPausedScene(ECSOrganizer &ecs,Shader *textShader);

void loadWinningScene(ECSOrganizer &ecs,Shader *textShader, Shader *backgroundShader);

void loadChooseGameSceneScene(ECSOrganizer &ecs,Shader *textShader, Shader *backgroundShader);

void loadSettingsScene(ECSOrganizer &ecs,Shader *textShader, Shader *squareShader, Shader *backgroundShader);

void loadBackground(ECSOrganizer &ecs, Shader *backgroundShader, BackgroundType bType);

