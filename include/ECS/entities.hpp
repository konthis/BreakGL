#pragma once
#include "signatures.hpp" 
#include <iostream>
#include <cassert>
#include <queue>
#include <array>

using Entity                = uint32_t;
const Entity MAX_ENTITIES   = 5000;

class EntityManager{
    private:
    std::queue<Entity> mAvailableEntities{};
    std::array<Signature, MAX_ENTITIES> mSignatures{};
    uint32_t mLivingEntitiesCount{};
    public:
    EntityManager(){
        for(Entity e=0; e<MAX_ENTITIES; e++){
            mAvailableEntities.push(e);
        }
    }

    Entity createEntity(){
        assert(mLivingEntitiesCount < MAX_ENTITIES && "Max Entities reached.");
        Entity entity = mAvailableEntities.front();
        mAvailableEntities.pop();
        mLivingEntitiesCount++;
        return entity;
    }

    void destroyEntity(const Entity &entity){
        assert(entity < MAX_ENTITIES && "Entity out of range.");
        mSignatures[entity].reset();
        mAvailableEntities.push(entity);
        mLivingEntitiesCount--;
    }

    void setSignature(const Entity &entity, const Signature &signature){
        assert(entity < MAX_ENTITIES && "Entity out of range.");
        mSignatures[entity] = signature;
    }

    Signature &getSignature(const Entity &entity){
        assert(entity < MAX_ENTITIES && "Entity out of range.");
        return mSignatures[entity]; 
    }

    void printEntity(const Entity &entity){
        std::cout << "Entity " << entity << "\nSignature " << getSignature(entity) << "\n";
    }
};
