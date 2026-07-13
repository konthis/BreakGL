#pragma once
#include <glad/glad.h>
#include <glm/vec4.hpp>

constexpr glm::vec4 COLOR_WHITE                 = {1.0f,1.0f,1.0f,1.0f};
constexpr glm::vec4 COLOR_GREEN                 = {0.0f,1.0f,0.0f,1.0f};
constexpr glm::vec4 BACKGROUND_GREEN            = {0.15, 0.45, 0.15, 0.8};
constexpr glm::vec4 BACKGROUND_ORANGE           = {0.55, 0.35, 0.1, 1.0};
constexpr glm::vec4 BACKGROUND_GRAY             = {0.3, 0.3, 0.3, 1.0};
constexpr glm::vec4 BACKGROUND_DARK_NAVY        = {0.05, 0.05, 0.15, 1.0};
constexpr glm::vec4 BACKGROUND_CHARCOAL         = {0.1, 0.1, 0.1, 1.0};
constexpr glm::vec4 BACKGROUND_DARK_TEAL        = {0.02, 0.10, 0.12, 1.0};
constexpr glm::vec4 BACKGROUND_DARK_PURPLE      = {0.05, 0.02, 0.10, 1.0};
constexpr glm::vec4 OVERLAY_ALPHA_MASK_MID      = {0.0, 0.0, 0.0, 0.5};
constexpr glm::vec4 SQUARE_COLOR_RED            = {0.75f,0.0f,0.0f,1.0f};
constexpr glm::vec4 SQUARE_COLOR_YELLOW         = {0.75f,0.75f,0.0f,1.0f};
constexpr glm::vec4 SQUARE_COLOR_GREEN          = {0.0f,0.6f,0.0f,1.0f};
constexpr glm::vec4 MENU_SELECTED_TEXT_COLOR    = {0.035f, 0.522f, 0.0f, 1.0f};
constexpr glm::vec4 MENU_TEXT_COLOR             = {0.345, 0.0, 0.431, 1.0};

constexpr GLuint HUD_HEIGHT     = 50;
constexpr GLuint WINDOW_HEIGHT  = 600;
constexpr GLuint WINDOW_WIDTH   = 800;

constexpr float BALL_RADIUS         = 6.0f;
constexpr float BALL_ABS_SPEED      = 350.0f;
constexpr float PLATFORM_SIDE_SMALL = 10.0f;
constexpr float PLATFORM_SIDE_BIG   = 100.0f;
constexpr float PLATFORM_SIDE_BIG_POWERUP = 200.0f;
constexpr float SQUARE_SIDE         = 20.0f;
constexpr float SQUARE_GAP          = 0.0f;

constexpr float PLATFORM_WIDE_TIMER = 5.0f;

constexpr const char * MUSIC_SCENE_MENU     = "audio/Next_to_you.mp3";
constexpr const char * MUSIC_SCENE_1        = "audio/Interplanetary_Odyssey.mp3";
constexpr const char * MUSIC_SCENE_2        = "audio/Heroic_Intrusion.mp3";
constexpr const char * MUSIC_SCENE_3        = "audio/slow-travel.mp3";
constexpr const char * MUSIC_WIN            = "audio/win.mp3";
constexpr const char * MUSIC_LOSE           = "audio/lose.mp3";
constexpr const char * SFX_TEST             = "audio/sfxtest.wav";
constexpr const char * SFX_BOUNCE           = "audio/bounce.wav";
constexpr const char * SFX_SPAWN_BALL       = "audio/ballspawn.wav";
constexpr const char * SFX_WIDE_PLATFORM    = "audio/wideplatform.wav";

constexpr const unsigned int SFX_BOUNCE_ID          = 0;
constexpr const unsigned int SFX_SPAWN_BALL_ID      = 1;
constexpr const unsigned int SFX_WIDE_PLATFORM_ID   = 2;
constexpr const unsigned int SFX_TEST_ID            = 3;

enum PowerUp {
    EMPTY,
    SUMMON_BALL,
    LONGER_PLATFORM,
 };


enum GameState {
    MainMenu, 
    ChooseSceneMenu, 
    Playing, 
    Paused, 
    PausedSettings,
    Settings,
    GameOver, 
    Win 
};

enum GameScene{
    Scene1,
    Scene2,
    Scene3
};

enum MenuType { 
    Vertical, 
    Horizontal, 
    SelectAndBar,
};

enum BarType{
    Music,
    Sfx
};

enum BackgroundType{
    SCENE_1,
    SCENE_2,
    SCENE_3,
    MAIN_MENU,
    WIN,
    LOSE
};