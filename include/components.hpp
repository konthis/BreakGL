#pragma once
#include "entities.hpp"
#include <unordered_map>


template<typename T>
class ComponentArray{
    private:
        // 2 u_maps implementation, maybe check another one later
        std::array<T,MAX_ENTITIES> mComponentArray{};
        std::unordered_map<Entity,size_t> mEntityToIndexMap;
        std::unordered_map<size_t,Entity> mIndexToEntityMap;
        size_t mSize{};
    public:
        ComponentArray()    = default;
        ~ComponentArray()   = default;
        void InsertComponent(const Entity &entity, const T &component){
            assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component for Entity already exists.");
            mEntityToIndexMap[entity]   = mSize;
            mIndexToEntityMap[mSize]    = entity;
            mComponentArray[mSize]      = component;
            mSize++;
        }

        void DeleteComponent(const Entity &entity){
            // have to swap last component to the deleted pos
            assert(mEntityToIndexMap.find(entity)!=mEntityToIndexMap.end() && "Entity's component doesn't exist");
            size_t indexRemovedComponent    = mEntityToIndexMap[entity];
            mComponentArray[indexRemovedComponent] = mComponentArray[mSize - 1];

            Entity lastEntity = mIndexToEntityMap[mSize - 1];
            mEntityToIndexMap[lastEntity] = indexRemovedComponent;
            mIndexToEntityMap[indexRemovedComponent] = lastEntity;

            mEntityToIndexMap.erase(entity);
            mEntityToIndexMap.erase(mSize -1);
            mSize--;
        }

        T& GetComponent(const Entity &entity){
            assert(mEntityToIndexMap.find(entity)!=mEntityToIndexMap.end() && "Entity's component doesn't exist");
            return mComponentArray[mEntityToIndexMap[entity]];
        }
};


class ComponentManager{

};