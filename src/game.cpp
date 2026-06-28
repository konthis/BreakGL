#include "game.hpp"

GLuint width    = 800;
GLuint height   = 600; 

ECSOrganizer ecs_org;

void Game::run(){ 
    mWindow.init(width, height, "BreakGL");
    Shader squareShader(SHADER_DIR "/square/square.vert",
                        SHADER_DIR "/square/square.frag");
    ecs_org.init();

    ecs_org.createComponent<Square>();
    ecs_org.createComponent<Position>();
    ecs_org.createComponent<Renderable>();

    auto meshGenSystem = ecs_org.createSystem<MeshGenSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Renderable>());
        ecs_org.setSystemSignature<MeshGenSystem>(sig);
    }


    auto renderSystem = ecs_org.createSystem<RenderSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Renderable>());
        ecs_org.setSystemSignature<RenderSystem>(sig);
    }
    renderSystem->init();

    std::vector<Entity> entities(1);
    for (auto& entity : entities){
        entity = ecs_org.createEntity();
        ecs_org.addComponent<Square>(entity,Square{
            .side = 0.1f,
            .color = glm::vec4{1.0f,0.0f,0.0f,1.0f},
        });
        ecs_org.addComponent<Position>(entity, Position{
            .position = glm::vec2{0.0f,0.0f},
        });
        ecs_org.addComponent<Renderable>(entity, Renderable{&squareShader});
    }

    meshGenSystem->init();


    while(!mWindow.shouldClose()){
        mWindow.pollEvents();

        renderSystem->update(0.1f);

        mWindow.swapBuffers();
    }
}

void Game::kill(){
    mWindow.kill();
}
