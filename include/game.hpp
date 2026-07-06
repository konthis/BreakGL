#pragma once
#include "window.hpp"
#include "shader.hpp"
#include "systems.hpp"
#include "scenes.hpp"
#include "game_state.hpp"
#include "text.hpp"


class Game{
    private:
        Window mWindow;
        GameState mState;
    public:
        void run();
        void kill();
};