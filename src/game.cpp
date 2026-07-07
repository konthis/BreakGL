#include "game.hpp"

ECSOrganizer ecs_org;

void Game::init(){
    mWindow.init(mWidth, mHeight, "BreakGL");
    mSimpleShader = std::make_unique<Shader>(SHADER_DIR "/simple/simple.vert", SHADER_DIR "/simple/simple.frag");

    mBallShader = std::make_unique<Shader>(SHADER_DIR "/ball/ball.vert",SHADER_DIR "/ball/ball.frag");

    mPlatformShader = std::make_unique<Shader>(SHADER_DIR "/platform/platform.vert",SHADER_DIR "/platform/platform.frag");

    mTextShader = std::make_unique<Shader>(SHADER_DIR "/text/text.vert", SHADER_DIR "/text/text.frag");

    // COMPONENTS
    ecs_org.init();

    ecs_org.createComponent<Ball>();
    ecs_org.createComponent<Text>();
    ecs_org.createComponent<Square>();
    ecs_org.createComponent<Platform>();
    ecs_org.createComponent<Position>();
    ecs_org.createComponent<Collider>();
    ecs_org.createComponent<RigidBody>();
    ecs_org.createComponent<Renderable>();
    ecs_org.createComponent<MenuOption>();
    ecs_org.createComponent<PlayerInput>();

    // SYSTEMS
    mMeshGenSystem = ecs_org.createSystem<MeshGenSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Renderable>());
        ecs_org.setSystemSignature<MeshGenSystem>(sig);
    }

    mPhysicsSystem = ecs_org.createSystem<PhysicsSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        // sig.set(ecs_org.getComponentType<Gravity>());
        sig.set(ecs_org.getComponentType<RigidBody>());
        ecs_org.setSystemSignature<PhysicsSystem>(sig);
    }


    mInputSystem = ecs_org.createSystem<InputSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<RigidBody>());
        sig.set(ecs_org.getComponentType<PlayerInput>());
        ecs_org.setSystemSignature<InputSystem>(sig);
    }
    mInputSystem->init();

    mCollisionSystem = ecs_org.createSystem<CollisionSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<RigidBody>());
        sig.set(ecs_org.getComponentType<Collider>());
        ecs_org.setSystemSignature<CollisionSystem>(sig);
    }
    mCollisionSystem->init(glm::vec4{0,mWidth,0,mHeight});


    mRenderSystem = ecs_org.createSystem<RenderSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Renderable>());
        ecs_org.setSystemSignature<RenderSystem>(sig);
    }
    mRenderSystem->init(mWidth,mHeight);

    mTextRenderSystem = ecs_org.createSystem<TextRenderSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<Position>());
        sig.set(ecs_org.getComponentType<Text>());
        ecs_org.setSystemSignature<TextRenderSystem>(sig);
    }

    mMenuInputSystem = ecs_org.createSystem<MenuInputSystem>();
    {
        Signature sig;
        sig.set(ecs_org.getComponentType<MenuOption>());
        sig.set(ecs_org.getComponentType<Text>());
        ecs_org.setSystemSignature<MenuInputSystem>(sig);
    }
    mMenuInputSystem->init();
    mPhysicsSystem->init();
    mMeshGenSystem->init();
    mTextRenderSystem->init(mWidth, mHeight, mTextShader.get(), 40);

    setGameState(GameState::MainMenu);
}

void Game::run(){ 

    float currTime = 0.f;
    float currFPS{};
    unsigned int countForFPS = 0;

    while(!mWindow.shouldClose()){
        


        mWindow.pollEvents();
        GLuint keyPressed = mWindow.processKeyPress(mState);
        switch(mState){
            case GameState::Playing:{
                currTime = (float)glfwGetTime();
                mTimeAccumulator += currTime - mLastTime;
                currFPS += (1.0f/(currTime-mLastTime))/30.0f;
                mLastTime = currTime;
                countForFPS++;
                if(countForFPS>=29){
                    countForFPS = 0;
                    auto & t = ecs_org.getComponent<Text>(eFPS);
                    t.content = std::to_string((int)currFPS);
                    currFPS = 0;
                }

                mInputSystem->update(keyPressed);

                while(mTimeAccumulator >= mDT){
                    mPhysicsSystem->update(mDT);
                    mTimeAccumulator -= mDT;
                }
                mCollisionSystem->update(mState);
                mRenderSystem->update();
                mTextRenderSystem->update();

                if(keyPressed==GLFW_KEY_ESCAPE){
                    setGameState(GameState::MainMenu);
                }
                break;
            }
            case GameState::MainMenu:{
                // mState = GameState::Playing;
                int confirmed = mMenuInputSystem->update(keyPressed,mDT/2.0f);
                mTextRenderSystem->update();
                if (confirmed==1){
                    setGameState(GameState::Playing);
                }
                else if (confirmed == 3){
                    exit(0);
                }
                break;
            }
            case GameState::ESCMenu:
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
                ecs_org.addComponent<Position>(ball, Position{ .position = {mWidth/2.0f, mWidth/5.0f} });
                ecs_org.addComponent<RigidBody>(ball, RigidBody{ .velocity = {200.0f, 250.0f}});
                ecs_org.addComponent<Collider>(ball,Collider{});
                ecs_org.addComponent<Renderable>(ball, Renderable{ mBallShader.get(), .color = {1.0f,1.0f,1.0f,1.0f} });
                mMeshGenSystem->initEntity(ball);
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


void Game::setGameState(GameState newState) {
    ecs_org.reset();
    switch(newState) {
        case GameState::Playing:{
            // FPS ENTITY
            eFPS = ecs_org.createEntity();
            ecs_org.addComponent<Text>(eFPS,Text{
                .content = "",
                .color = glm::vec4{0.0f,1.0f,0.0f,1.0f},
                .scale = 0.5f
            });
            ecs_org.addComponent<Position>(eFPS, Position{ 
                .position = glm::vec2{50.0f,mHeight-mHUDheight + 15.0f}
            });
            //
            loadSceneBallPlatformSquares(ecs_org, mBallShader.get(),mPlatformShader.get(), mSimpleShader.get(), glm::vec2{mWidth,mHeight}, mHUDheight);
            mMeshGenSystem->init();
            break;
        }    
        case GameState::MainMenu:{
            glClear(GL_COLOR_BUFFER_BIT);
            loadMainMenuScene(ecs_org, mTextShader.get(), glm::vec2{mWidth,mHeight});
            break;
        }
        case GameState::ESCMenu:{
            break;
        }
        case GameState::Paused:{
            break;
        }
        case GameState::Win:{
            break;
        }
    }
    // reset times
    mLastTime = (float)glfwGetTime();
    mTimeAccumulator = 0.0f;
    mState = newState;
}
