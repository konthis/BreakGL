#include "scenes.hpp"

void loadScene1(ECSOrganizer& ecs, Shader *ballShader) {
    std::mt19937 rng(1337);
    std::uniform_real_distribution<float> posX(50.f, 750.f);
    std::uniform_real_distribution<float> posY(50.f, 550.f);
    std::uniform_real_distribution<float> vel(-200.f, 200.f);

    for (int i = 0; i < 20; i++) {
        Entity e = ecs.createEntity();
        ecs.addComponent<Ball>(e, Ball{ .radius = 20.f });
        ecs.addComponent<Gravity>(e, Gravity{ .value= 0.f });
        ecs.addComponent<Position>(e, Position{ .position = {posX(rng), posY(rng)} });
        ecs.addComponent<RigidBody>(e, RigidBody{ .velocity = {vel(rng), vel(rng)} });
        ecs.addComponent<Renderable>(e, Renderable{ ballShader, .color = {1,1,1,1} });
        ecs.addComponent<PlayerInput>(e,PlayerInput{});
    }
}
