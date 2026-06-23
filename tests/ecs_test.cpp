#include "systems.hpp"
#include <vector>
#include <random>
#include <glm/vec2.hpp>

int main(){
    ecs_org.createComponent<Position>();    
    ecs_org.createComponent<RigidBody>();    
    ecs_org.createComponent<Gravity>();    

    ecs_org.createSystem<PhysicsSystem>();
    Signature signature;
	signature.set(ecs_org.getComponentType<Gravity>());
	signature.set(ecs_org.getComponentType<RigidBody>());
	signature.set(ecs_org.getComponentType<Position>());
	ecs_org.setSystemSignature<PhysicsSystem>(signature);


    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
    std::uniform_real_distribution<float> randAcceleration(-1.0f, 1.0f);

    std::vector<Entity> entities;
    for(Entity e:entities){
        e = ecs_org.createEntity();
        ecs_org.addComponent<Gravity>(e,{});
        ecs_org.addComponent<RigidBody>(e,
            RigidBody{.velocity = glm::vec2{0.0,0.0},.acceleration = {0.,randAcceleration(generator)}});
        ecs_org.addComponent<Position>(e,Position{.pos = glm::vec2{randPosition(generator),randPosition(generator)}});
    }
    
    return 0;
}