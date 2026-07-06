#include "scenes.hpp"

void loadSceneBalls(ECSOrganizer& ecs, Shader *ballShader) {
    std::mt19937 rng(1337);
    std::uniform_real_distribution<float> posX(50.f, 750.f);
    std::uniform_real_distribution<float> posY(50.f, 550.f);
    std::uniform_real_distribution<float> vel(-200.f, 200.f);
    std::uniform_real_distribution<float> col(0.f, 1.f);

    for (int i = 0; i < 20; i++) {
        Entity e = ecs.createEntity();
        ecs.addComponent<Ball>(e, Ball{ .radius = 15.f });
        ecs.addComponent<Gravity>(e, Gravity{ .value= 0.f });
        ecs.addComponent<Position>(e, Position{ .position = {posX(rng), posY(rng)} });
        ecs.addComponent<RigidBody>(e, RigidBody{ .velocity = {vel(rng), vel(rng)} });
        ecs.addComponent<Renderable>(e, Renderable{ ballShader, .color = {col(rng),col(rng),col(rng),1.0f} });
        ecs.addComponent<PlayerInput>(e,PlayerInput{});
    }
}


void loadSceneBallsAndPlatform(ECSOrganizer& ecs, Shader *ballShader, Shader *platformShader, glm::vec2 windowSize){

    std::mt19937 rng(1337);
    std::uniform_real_distribution<float> posX(50.f, 750.f);
    std::uniform_real_distribution<float> posY(50.f, 550.f);
    std::uniform_real_distribution<float> vel(-200.f, 200.f);
    std::uniform_real_distribution<float> col(0.f, 1.f);
    for (int i = 0; i < 20; i++) {
        Entity e = ecs.createEntity();
        ecs.addComponent<Ball>(e, Ball{ .radius = 15.f });
        // ecs.addComponent<Gravity>(e, Gravity{ .value= 0.f });
        ecs.addComponent<Position>(e, Position{ .position = {posX(rng), posY(rng)} });
        ecs.addComponent<RigidBody>(e, RigidBody{ .velocity = {vel(rng), vel(rng)} });
        ecs.addComponent<Collider>(e,Collider{});
        ecs.addComponent<Renderable>(e, Renderable{ ballShader, .color = {col(rng),col(rng),col(rng),1.0f} });
    }
    Entity e = ecs.createEntity();
    Platform platform = Platform{.smallSide = 15.0, .bigSide = 100.0f};
    ecs.addComponent<Platform>(e, platform);
    ecs.addComponent<Position>(e, Position{ 
        .position = {windowSize.x/2.0f, platform.smallSide/2.0f}});
    ecs.addComponent<RigidBody>(e, RigidBody{ .velocity = {0.0f, 0.0f}});
    ecs.addComponent<Collider>(e,Collider{});
    ecs.addComponent<Renderable>(e, Renderable{ platformShader, .color = {col(rng),col(rng),col(rng),1.0f} });
    ecs.addComponent<PlayerInput>(e,PlayerInput{});

}


void loadSceneBallPlatformSquares(ECSOrganizer& ecs, Shader *ballShader, Shader *platformShader, Shader *squareShader, glm::vec2 windowSize, GLuint HUDheight){
    std::mt19937 rng(1337);
    std::uniform_real_distribution<float> col(0.f, 1.f);
    std::uniform_real_distribution<float> ballVelDirAngleRNG(glm::pi<float>()/3.0f, 5.0f * glm::pi<float>()/6.0f);

    // platform
    Entity e = ecs.createEntity();
    Platform platform = Platform{.smallSide = 15.0, .bigSide = 100.0f};
    ecs.addComponent<Platform>(e, platform);
    ecs.addComponent<Position>(e, Position{ 
        .position = {windowSize.x/2.0f, platform.smallSide/2.0f}});
    float ballVelDirAngle = ballVelDirAngleRNG(rng);
    ecs.addComponent<RigidBody>(e, RigidBody{ 
        .velocity = glm::vec2{glm::cos(ballVelDirAngle),glm::sin(ballVelDirAngle)} * 150.0f
    });
    ecs.addComponent<Collider>(e,Collider{});
    ecs.addComponent<Renderable>(e, Renderable{ platformShader, .color = {col(rng),col(rng),col(rng),1.0f} });
    ecs.addComponent<PlayerInput>(e,PlayerInput{});
    //

    // ball
    e = ecs.createEntity();
    ecs.addComponent<Ball>(e, Ball{ .radius = 10.f });
    // ecs.addComponent<Gravity>(e, Gravity{ .value= 0.f });
    ecs.addComponent<Position>(e, Position{ .position = {windowSize.x/2.0f, windowSize.y/5.0f} });
    ecs.addComponent<RigidBody>(e, RigidBody{ .velocity = {0.0f, 250.0f}});
    ecs.addComponent<Collider>(e,Collider{});
    ecs.addComponent<Renderable>(e, Renderable{ ballShader, .color = {col(rng),col(rng),col(rng),1.0f} });
    //

    // squares
    GLfloat squareSide = 19.0f;
    BlockLayout bl = BlockLayout{
        .start = glm::vec2{squareSide/2.0f,(windowSize.y-HUDheight)-squareSide/2.0f},
        .size = glm::vec2{squareSide,squareSide},
        .gap = glm::vec2{1.0f,1.0f},
        .cols = 40,
        .rows = 10,
        .max = 150
    };
    for (int i = 0; i < 400; i++) {
        Entity e = ecs.createEntity();
        ecs.addComponent<Square>(e, Square{ .side = squareSide});
        ecs.addComponent<Position>(e, Position{ .position = bl.next()});
        ecs.addComponent<RigidBody>(e, RigidBody{
            .velocity = glm::vec2{0.0f,0.0f},
            .acceleration = glm::vec2{0.0f,0.0f}
        });
        ecs.addComponent<Collider>(e,Collider{});
        ecs.addComponent<Renderable>(e, Renderable{ squareShader, .color = {col(rng),col(rng),col(rng),1.0f} });
    }
    //

    // text
    e = ecs.createEntity();
    ecs.addComponent<Text>(e,Text{
        .content = "FPS: ",
        .color = glm::vec4{0.0f,1.0f,0.0f,1.0f},
        .scale = 0.8f
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{0.0f,windowSize.y-HUDheight + 15.0f}
    });


}

void loadTextScene(ECSOrganizer& ecs, Shader *textShader, glm::vec2 windowSize){
    Entity e = ecs.createEntity();
    ecs.addComponent<Text>(e,Text{
        .content = "test",
        .color = glm::vec4{1.0f,1.0f,1.0f,1.0f},
        .scale = 1.0f
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = glm::vec2{200.0f,250.0f}
    });

}
