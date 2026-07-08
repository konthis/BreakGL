#pragma once
#include <glad/glad.h>
#include <glm/vec4.hpp>

constexpr glm::vec4 COLOR_WHITE                 = {1.0f,1.0f,1.0f,1.0f};
constexpr glm::vec4 COLOR_GREEN                 = {0.0f,1.0f,0.0f,1.0f};
constexpr glm::vec4 SQUARE_COLOR_RED            = {0.75f,0.0f,0.0f,1.0f};
constexpr glm::vec4 SQUARE_COLOR_YELLOW         = {0.75f,0.75f,0.0f,1.0f};
constexpr glm::vec4 MENU_SELECTED_TEXT_COLOR    = {0.035f, 0.522f, 0.0f, 1.0f};
constexpr glm::vec4 MENU_TEXT_COLOR             = {0.345, 0.0, 0.431, 1.0};

constexpr GLuint HUD_HEIGHT     = 50;
constexpr GLuint WINDOW_HEIGHT  = 600;
constexpr GLuint WINDOW_WIDTH   = 800;

constexpr float BALL_RADIUS         = 8.0f;
constexpr float BALL_ABS_SPEED      = 300.0f;
constexpr float PLATFORM_SIDE_SMALL = 10.0f;
constexpr float PLATFORM_SIDE_BIG   = 100.0f;
constexpr float SQUARE_SIDE         = 18.0f;
constexpr float SQUARE_GAP          = 2.0f;

enum PowerUp {
    EMPTY,
    SUMMON_BALL,
    LONGER_PLATFORM,
 };