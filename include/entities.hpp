#pragma once
#include <iostream>
#include <cassert>
#include <queue>
#include <array>
#include <bitset>

using Entity                = uint32_t;
using ComponentType         = uint8_t;
using Signature             = std::bitset<MAX_COMPONENTS>;
const Entity MAX_ENTITIES   = 5000;
const ComponentType MAX_COMPONENTS = 32;

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

    Entity CreateEntity(){
        assert(mLivingEntitiesCount < MAX_ENTITIES && "Max Entities reached.");
        Entity entity = mAvailableEntities.front();
        mAvailableEntities.pop();
        mLivingEntitiesCount++;
        return entity;
    }

    void DestroyEntity(const Entity &entity){
        assert(entity < MAX_ENTITIES && "Entity out of range.");
        mSignatures[entity].reset();
        mAvailableEntities.push(entity);
        mLivingEntitiesCount--;
    }

    void SetSignature(const Entity &entity, const Signature &signature){
        assert(entity < MAX_ENTITIES && "Entity out of range.");
        mSignatures[entity] = signature;
    }
    Signature &GetSignature(const Entity &entity){
        assert(entity < MAX_ENTITIES && "Entity out of range.");
        return mSignatures[entity]; 
    }

};
