#pragma once
#include "window.hpp"
#include "shader.hpp"
#include "systems.hpp"
#include "scenes.hpp"


class Game{
    private:
        Window mWindow;
    public:
        void run();
        void kill();
};