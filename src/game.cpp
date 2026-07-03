#include "game.hpp"

GLuint width    = 800;
GLuint height   = 600; 
const float DT  = 1/120.f;

ECSOrganizer ecs_org;



void Game::run(){ 
    mWindow.init(width, height, "BreakGL");
    Shader simpleShader(SHADER_DIR "/simple/simple.vert",
                        SHADER_DIR "/simple/simple.frag");

    Shader ballShader(SHADER_DIR "/ball/ball.vert",
                        SHADER_DIR "/ball/ball.frag");

    Shader platformShader(SHADER_DIR "/platform/platform.vert",
                          SHADER_DIR "/platform/platform.frag");
    ecs_org.init();

    ecs_org.createComponent<Square>();
    ecs_org.createComponent<Platform>();
    ecs_org.createComponent<Ball>();
    ecs_org.createComponent<Position>();
    ecs_org.createComponent<Gravity>();
    ecs_org.createComponent<RigidBody>();
    ecs_org.createComponent<Collider>();
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
        sig.set(ecs_org.getComponentType<RigidBody>());
        ecs_org.setSystemSignature<PhysicsSystem>(sig);
    }


    auto inputSystem = ecs_org.createSystem<InputSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<PlayerInput>());
        ecs_org.setSystemSignature<InputSystem>(sig);
    }
    inputSystem->init();

    auto collisionSystem = ecs_org.createSystem<CollisionSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<RigidBody>());
        sig.set(ecs_org.getComponentType<Collider>());
        ecs_org.setSystemSignature<CollisionSystem>(sig);
    }
    collisionSystem->init(glm::vec4{0,width,0,height});


    auto renderSystem = ecs_org.createSystem<RenderSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Renderable>());
        ecs_org.setSystemSignature<RenderSystem>(sig);
    }
    renderSystem->init(width,height);

    // loadSceneBalls(ecs_org, &ballShader);
    loadSceneBallsAndPlatform(ecs_org, &ballShader, &platformShader, glm::vec2{width,height});
    physicsSystem->init();
    meshGenSystem->init();


    float currTime = 0.f, lastTime = 0.f, dt=0.f;
    float timeAccumulator;
    while(!mWindow.shouldClose()){
        
        currTime = (float)glfwGetTime();
        timeAccumulator += currTime - lastTime;
        lastTime = currTime;

        mWindow.pollEvents();
        
        inputSystem->update(mWindow);

        while(timeAccumulator >= DT){
            physicsSystem->update(DT);
            timeAccumulator -= DT;
        }

        collisionSystem->update();
        renderSystem->update();

        mWindow.swapBuffers();
    }
}

void Game::kill(){
    mWindow.kill();
}
