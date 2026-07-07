/*
    All actions on the loop, comes from the organizer, add, remove w/e. Its just a wrapper
*/
#pragma once
#include "system_manager.hpp"
#include "components.hpp"


class ECSOrganizer{
    private:
	std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<SystemManager> mSystemManager;

    public:
    void init(){
	    mEntityManager      = std::make_unique<EntityManager>();
        mComponentManager   = std::make_unique<ComponentManager>();
	    mSystemManager      = std::make_unique<SystemManager>();
    }

    Entity createEntity(){
        return mEntityManager->createEntity();
    }

    Signature getSignature(const Entity &entity){
        return mEntityManager->getSignature(entity);
    }

    void destroyEntity(const Entity &entity){
        mEntityManager->destroyEntity(entity);
        mComponentManager->entityDestroyed(entity);
        mSystemManager->entityDestroyed(entity);
    }

    void printEntity(const Entity &entity){
        mEntityManager->printEntity(entity);
    }

    template <typename T>
    void createComponent(){
        mComponentManager->createComponentType<T>();
    }

    template <typename T>
    void addComponent(const Entity &entity, const T &component){
        mComponentManager->addComponent<T>(entity,component);
        Signature signature = mEntityManager->getSignature(entity);
        signature.set(mComponentManager->getComponentType<T>(),true);
        mEntityManager->setSignature(entity,signature);
        mSystemManager->updateEntitiesOnSystems(entity,signature);
    }

    template <typename T>
    void removeComponent(const Entity &entity){
        mComponentManager->removeComponent<T>(entity);
        Signature signature = mEntityManager->getSignature(entity);
        signature.set(mComponentManager->getComponentType<T>(),false);
        mEntityManager->setSignature(entity,signature);
        mSystemManager->updateEntitiesOnSystems(entity,signature);
    }

    template<typename T>
	T& getComponent(const Entity &entity)
	{
		return mComponentManager->getComponent<T>(entity);
	}

	template<typename T>
	ComponentType getComponentType()
	{
		return mComponentManager->getComponentType<T>();
	}

    template<typename T>
	std::shared_ptr<T> createSystem()
	{
		return mSystemManager->createSystem<T>();
	}

	template<typename T>
	void setSystemSignature(const Signature &signature)
	{
		mSystemManager->setSystemSignature<T>(signature);
	}

    void reset() {
        // remove all entities
        auto alive = mEntityManager->getLivingEntities();
        for (auto e : alive)
            destroyEntity(e);
    }

};