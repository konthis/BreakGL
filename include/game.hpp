#pragma once
#include "constants.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "systems.hpp"
#include "scenes.hpp"
#include "text.hpp"


class Game{
    private:
        Window mWindow;
        GameState mState;
        GLuint mHUDheight    = 50;
        const float mDT      = 1/240.f;
        // shaders
        std::unique_ptr<Shader> mBallShader;
        std::unique_ptr<Shader> mSimpleShader;
        std::unique_ptr<Shader> mPlatformShader;
        std::unique_ptr<Shader> mTextShader;
        // systems
        std::shared_ptr<MeshGenSystem>   mMeshGenSystem;
        std::shared_ptr<PhysicsSystem>   mPhysicsSystem;
        std::shared_ptr<InputSystem>     mInputSystem;
        std::shared_ptr<CollisionSystem> mCollisionSystem;
        std::shared_ptr<RenderSystem>    mRenderSystem;
        std::shared_ptr<TextRenderSystem> mTextRenderSystem;
        std::shared_ptr<MenuInputSystem> mMenuInputSystem;
        // collision results
        CollisionResult mCr;
        // FPS entity
        Entity eFPS;
        // time
        Entity eTimer;
        float mTimerAccumulator{}; // for the timer only
        float mLastTime{};
        float mTimeAccumulator{};
        // balls
        GLuint mBallCount = 0;
        // platform
        Entity mPlatformEntity;
        // Platform mPlatformComp;
        // scene
        GameScene mGameScene = GameScene::Scene1;
        int mLastPreviewIdx;


        void setGameState(GameState newState);
    public:
        void init();
        void run();
        void kill();
};