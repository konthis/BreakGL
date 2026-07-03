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
        ecs.addComponent<Gravity>(e, Gravity{ .value= 0.f });
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
