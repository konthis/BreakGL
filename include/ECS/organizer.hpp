/*
    All actions on the loop, comes from the organizer, add, remove w/e. Its just a wrapper
*/

#pragma once
#include "systems.hpp"
#include "components.hpp"


class Organizer{
    private:
	std::unique_ptr<EntityManager> mEntityManager = std::make_unique<EntityManager>();
    std::unique_ptr<ComponentManager> mComponentManager = std::make_unique<ComponentManager>();
	std::unique_ptr<SystemManager> mSystemManager = std::make_unique<SystemManager>();
    std::unique_ptr<SignatureManager> mSignatureManager = std::make_unique<SignatureManager>();

    public:
    Entity createEntity(){

        return mEntityManager->createEntity();
    }

    void destroyEntity(const Entity &entity){
        mEntityManager->destroyEntity(entity);
    }
    
};