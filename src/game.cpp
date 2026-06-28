#include "game.hpp"

GLuint width    = 800;
GLuint height   = 600; 

ECSOrganizer ecs_org;

void Game::init(){
    mWindow.init(width, height, "BreakGL");
    ecs_org.init();

    ecs_org.createComponent<Ball>();
    ecs_org.createComponent<Transform>();
    ecs_org.createComponent<Renderable>();


    auto renderSystem = ecs_org.createSystem<RenderSystem>();
    {
        Signature sig;
        /////
    }
    renderSystem->init();
}

void Game::run(){
    while(!mWindow.shouldClose()){
        mWindow.pollEvents();
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        update(0.1);

        mWindow.swapBuffers();
    }
}

void Game::kill(){
    mWindow.kill();
}

void Game::update(GLfloat dt){

}
