#include "game.hpp"

ECSOrganizer ecs_org;

void Game::init(){
    mWindow.init(WINDOW_WIDTH, WINDOW_HEIGHT, "BreakGL");
    // mWindow.init(mWidth, mHeight, "BreakGL");
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
    ecs_org.createComponent<PauseMenu>();
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

    std::set<Entity> toDestroy; // for multiple balls -> multiple squuare deletions
    std::set<unsigned int> toCreate{}; // powerup

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
                if(countForFPS>29){
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
                mCollisionSystem->update(mState, toDestroy, toCreate);
                // 
                for (auto e : toDestroy){
                    auto const& sig = ecs_org.getSignature(e);
                    if(sig.test(ecs_org.getComponentType<Ball>())){
                        mBallCount--;
                    }
                    ecs_org.destroyEntity(e);
                }
                if(!mBallCount){
                    setGameState(GameState::GameOver);
                }

                for (auto& e: toCreate) {
                    auto const& sig = ecs_org.getSignature(e);
                   if(sig.test(ecs_org.getComponentType<Ball>()))
                    mBallCount++;
                    auto& pos = ecs_org.getComponent<Position>(e);
                    mMeshGenSystem->initEntity(e);
                }

                toDestroy.clear();
                toCreate.clear();

                mRenderSystem->update();
                mTextRenderSystem->update();

                if(keyPressed==GLFW_KEY_ESCAPE){
                    setGameState(GameState::Paused);
                }
                break;
            }
            case GameState::MainMenu:{
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
            case GameState::Paused:{
                int confirmed = mMenuInputSystem->update(keyPressed,mDT/2.0f);
                mTextRenderSystem->update();
                if (confirmed == 0){
                    // just continue, but still reset the times
                    mLastTime = (float)glfwGetTime();
                    mTimeAccumulator = 0.0f;
                    for(auto &e : ecs_org.getEntitiesOfComponent<PauseMenu>()){
                        ecs_org.destroyEntity(e);
                    }
                    mState = GameState::Playing;

                }
                else if (confirmed == 1){
                    setGameState(GameState::MainMenu);
                }
                break;
            }
            case GameState::GameOver:{
                int confirmed = mMenuInputSystem->update(keyPressed,mDT/2.0f);
                mTextRenderSystem->update();
                if (confirmed==0){
                    setGameState(GameState::MainMenu);
                }
                else if (confirmed == 1){
                    exit(0);
                }
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
    switch(newState) {
        case GameState::Playing:{
            ecs_org.reset();
            mBallCount = 0;
            // FPS ENTITY
            eFPS = createText(ecs_org,"",

                glm::vec4{0.0f,1.0f,0.0f,1.0f},
                0.5f,
                glm::vec2{50.0f, WINDOW_HEIGHT-HUD_HEIGHT+25.0f}
            );
            loadScene1(ecs_org, mBallShader.get(),mPlatformShader.get(), mSimpleShader.get(), glm::vec2{mWidth,mHeight}, mHUDheight);
            mBallCount++;
            mMeshGenSystem->init();
            break;
        }    
        case GameState::MainMenu:{
            ecs_org.reset();
            glClear(GL_COLOR_BUFFER_BIT);
            loadMainMenuScene(ecs_org, mTextShader.get(), glm::vec2{mWidth,mHeight});
            break;
        }
        case GameState::Paused:{
            loadPausedScene(ecs_org, mTextShader.get());
            break;
        }
        case GameState::GameOver:{
            ecs_org.reset();
            loadGameOverScene(ecs_org,mTextShader.get());
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
