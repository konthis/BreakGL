#pragma once
#include "window.hpp"
#include "shader.hpp"
#include "systems.hpp"


class Game{
    private:
        Window mWindow;
        void update(GLfloat dt);
        void render();
    public:
        void init();
        void run();
        void kill();
};