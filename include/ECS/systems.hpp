#pragma once
#include "system_manager.hpp"
#include "ECSorganizer.hpp"
#include "components.hpp"

ECSOrganizer ecs_org;

class PhysicsSystem: public System{
    private:
    public:
    void update(float dt){
    for (auto const& entity : mEntities)
    	{
    		auto& rigidBody = ecs_org.getComponent<RigidBody>(entity);
    		auto& position = ecs_org.getComponent<Position>(entity);
    		auto const& gravity = ecs_org.getComponent<Gravity>(entity);

    		position.pos+= rigidBody.velocity * dt;

    		rigidBody.velocity += rigidBody.acceleration * gravity.g * dt;
			rigidBody.acceleration += gravity.g * dt / rigidBody.mass;
    	}
    }
};