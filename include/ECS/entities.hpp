#pragma once
#include <iostream>
#include <cassert>
#include <queue>
#include <array>
#include <bitset>

using Entity                = uint32_t;
const Entity MAX_ENTITIES   = 5000;
using ComponentType                 = uint8_t;
const ComponentType MAX_COMPONENTS  = 32;
using Signature                     = std::bitset<MAX_COMPONENTS>;



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

    std::vector<Entity> getLivingEntities() {
        std::vector<Entity> alive;
        for (Entity e = 0; e < MAX_ENTITIES; e++)
            if (mSignatures[e].any())
                alive.push_back(e);
        return alive;
    }
};
