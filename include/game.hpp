#pragma once
#include "window.hpp"
#include "shader.hpp"
#include "systems.hpp"


class Game{
    private:
        Window mWindow;
    public:
        void run();
        void kill();
};