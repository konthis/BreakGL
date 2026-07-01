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
    ecs_org.createComponent<Gravity>();
    ecs_org.createComponent<Renderable>();
    ecs_org.createComponent<PlayerInput>();

    auto meshGenSystem = ecs_org.createSystem<MeshGenSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Renderable>());
        ecs_org.setSystemSignature<MeshGenSystem>(sig);
    }

    auto physicsSystem = ecs_org.createSystem<PhysicsSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Gravity>());
        ecs_org.setSystemSignature<PhysicsSystem>(sig);
    }

    physicsSystem->init();

    auto inputSystem = ecs_org.createSystem<InputSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Gravity>());
        sig.set(ecs_org.getComponentType<PlayerInput>());
        ecs_org.setSystemSignature<InputSystem>(sig);
    }
    inputSystem->init();


    auto renderSystem = ecs_org.createSystem<RenderSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Renderable>());
        ecs_org.setSystemSignature<RenderSystem>(sig);
    }
    renderSystem->init(width,height);

    std::vector<Entity> entities(1);
    for (auto& entity : entities){
        entity = ecs_org.createEntity();
        ecs_org.addComponent<Square>(entity,Square{
            .side = 50.0f,
        });
        ecs_org.addComponent<Position>(entity, Position{
            .position = glm::vec2{400.0f,300.0f},
        });
        ecs_org.addComponent<Renderable>(entity, Renderable{&squareShader,
            .color = glm::vec4{1.0f,1.0f,0.0f,1.0f},}
        );

        ecs_org.addComponent<Gravity>(entity, Gravity{.g = 5.0f});
        ecs_org.addComponent<PlayerInput>(entity,PlayerInput{});
    }

    meshGenSystem->init();


    while(!mWindow.shouldClose()){
        mWindow.pollEvents();

        inputSystem->update(mWindow);
        physicsSystem->update(0.1f);
        renderSystem->update();

        mWindow.swapBuffers();
    }
}

void Game::kill(){
    mWindow.kill();
}
