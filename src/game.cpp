#include "game.hpp"

GLuint width        = 800;
GLuint height       = 600; 
GLuint HUDheight    = 50;
const float DT      = 1/240.f;

ECSOrganizer ecs_org;


void Game::run(){ 
    mWindow.init(width, height, "BreakGL");
    Shader simpleShader(SHADER_DIR "/simple/simple.vert",
                        SHADER_DIR "/simple/simple.frag");

    Shader ballShader(SHADER_DIR "/ball/ball.vert",
                        SHADER_DIR "/ball/ball.frag");

    Shader platformShader(SHADER_DIR "/platform/platform.vert",
                          SHADER_DIR "/platform/platform.frag");

    Shader textShader(SHADER_DIR "/text/text.vert",
                          SHADER_DIR "/text/text.frag");

    // TextRenderer tr;
    // tr.init();

    ecs_org.init();

    ecs_org.createComponent<Square>();
    ecs_org.createComponent<Platform>();
    ecs_org.createComponent<Ball>();
    ecs_org.createComponent<Position>();
    // ecs_org.createComponent<Gravity>();
    ecs_org.createComponent<RigidBody>();
    ecs_org.createComponent<Collider>();
    ecs_org.createComponent<Renderable>();
    ecs_org.createComponent<PlayerInput>();
    ecs_org.createComponent<Text>();

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
        // sig.set(ecs_org.getComponentType<Gravity>());
        sig.set(ecs_org.getComponentType<RigidBody>());
        ecs_org.setSystemSignature<PhysicsSystem>(sig);
    }


    auto inputSystem = ecs_org.createSystem<InputSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<RigidBody>());
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

    auto textRenderSystem = ecs_org.createSystem<TextRenderSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Text>());
        ecs_org.setSystemSignature<TextRenderSystem>(sig);
    }

    // loadSceneBalls(ecs_org, &ballShader);
    // loadSceneBallsAndPlatform(ecs_org, &ballShader, &platformShader, glm::vec2{width,height});
    // loadTextScene(ecs_org, &textShader, glm::vec2{width,height});
    loadSceneBallPlatformSquares(ecs_org, &ballShader,&platformShader, &simpleShader, glm::vec2{width,height}, HUDheight);
    physicsSystem->init();
    meshGenSystem->init();
    textRenderSystem->init(width, height, &textShader);


    float currTime = 0.f, lastTime = 0.f, dt=0.f;
    float timeAccumulator, currFPS{};
    unsigned int countForFPS = 0;
    // FPS ENTITY
    Entity e = ecs_org.createEntity();
    ecs_org.addComponent<Text>(e,Text{
        .content = "?",
        .color = glm::vec4{0.0f,1.0f,0.0f,1.0f},
        .scale = 0.8f
    });
    ecs_org.addComponent<Position>(e, Position{ 
        .position = glm::vec2{70.0f,height-HUDheight + 15.0f}
    });
    //
    mState = GameState::Playing;
    while(!mWindow.shouldClose()){
        
        currTime = (float)glfwGetTime();
        timeAccumulator += currTime - lastTime;
        currFPS += (1.0f/(currTime-lastTime))/30.0f;
        countForFPS++;
        if(countForFPS>=29){
            countForFPS = 0;
            auto & t = ecs_org.getComponent<Text>(e);
            t.content = std::to_string((int)currFPS);
            currFPS = 0;
        }
        lastTime = currTime;


        mWindow.pollEvents();
        GLuint keyPressed = mWindow.processKeyPress(mState);
        switch(mState){
            case GameState::Playing:{
                inputSystem->update(keyPressed);

                while(timeAccumulator >= DT){
                    physicsSystem->update(DT);
                    timeAccumulator -= DT;
                }
                collisionSystem->update(mState);
                renderSystem->update();
                textRenderSystem->update();
                break;
            }
            case GameState::Menu:
                mState = GameState::Playing;
                break;
            case GameState::Paused:
                mState = GameState::Playing;
                break;
            case GameState::GameOver:{
                // SPAWN A BALL AGAIN FOR NOW
                mState = GameState::Playing;
                Entity ball = ecs_org.createEntity();
                ecs_org.addComponent<Ball>(ball, Ball{ .radius = 10.f });
                ecs_org.addComponent<Position>(ball, Position{ .position = {width/2.0f, width/5.0f} });
                ecs_org.addComponent<RigidBody>(ball, RigidBody{ .velocity = {0.0f, 250.0f}});
                ecs_org.addComponent<Collider>(ball,Collider{});
                ecs_org.addComponent<Renderable>(ball, Renderable{ &ballShader, .color = {1.0f,1.0f,1.0f,1.0f} });

                meshGenSystem->initEntity(ball);
                break;

            }
            case GameState::Win:
                mState = GameState::Playing;
                break;

        }
        

        mWindow.swapBuffers();
    }
}

void Game::kill(){
    mWindow.kill();
}
