#pragma once
#include "entities.hpp"
#include "components.hpp"
#include <set>
#include <memory>
#include <unordered_map>

/* System-> apply things on a set of entities */

class System{
    public:
    std::set<Entity> mEntities;
};


class SystemManager{
    private:
    std::unordered_map<const char *, std::shared_ptr<System>> mSystems;
    // system signatures to check valid system entities
    std::unordered_map<const char*, Signature> mSignatures{};
    public:

    template<typename T>
	std::shared_ptr<T> createSystem()
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) == mSystems.end() && "Adding system more than once.");
		auto system = std::make_shared<T>();
		mSystems.insert({typeName, system});
		return system;
	}

    template<typename T>
	void setSystemSignature(const Signature &signature)
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");
		mSignatures.insert({typeName, signature});
	}

    void entityDestroyed(Entity entity)
	{
        // remove entity from all systems
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;
			system->mEntities.erase(entity);
		}
	}

    void updateEntitiesOnSystems(const Entity &entity, const Signature &entitySignature)
	{
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}
};
