#pragma once
#include "entities.hpp"
#include <set>
#include <memory>

/* System-> apply things on a set of entities */

class System{
    public:
    std::set<Entity> mEntities;
    Signature signature{};
};


class SystemManager{
    private:
    std::unordered_map<const char *, std::shared_ptr<System>> mSystems;
    // system signatures to check valid system entities
    std::unordered_map<const char*, Signature> mSignatures{};
    public:

    template<typename T>
	std::shared_ptr<T> addSystem()
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
};
