#pragma once
#include "entities.hpp"
#include "shader.hpp"
#include <unordered_map>
#include <memory>
#define GLM_ENABLE_EXPERIMENTAL // enables the to_string
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>



/*
ECS-COMPONENT MEMORY ARCHITECTURE: arrays of components
ex: component1 array = [comp1 of ent1, comp1 of ent2 ,....] on cont. mem
    component2 array = [comp2 of ent100, comp2 of ent3,...]
*/

// Have to use the Interface class, as in the component manager, as the component array class needs to be specific component type, and to create a map with the multiple difference component arrays, i need the types in advance, so init the map with the interface and then typecast when needed.

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
    virtual void deleteComponent(const Entity &entity){};// have to init the virtual with something
};

template<typename T>
class ComponentArray: public IComponentArray{
    private:
        // 2 u_maps implementation, maybe check another one later
        std::array<T,MAX_ENTITIES> mComponentArray{};
        std::unordered_map<Entity,size_t> mEntityToIndexMap;
        std::unordered_map<size_t,Entity> mIndexToEntityMap;
        size_t mSize{};
    public:
        ComponentArray()    = default;
        ~ComponentArray()   = default;
        void insertComponent(const Entity &entity, const T &component){
            assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component for Entity already exists.");
            mEntityToIndexMap[entity]   = mSize;
            mIndexToEntityMap[mSize]    = entity;
            mComponentArray[mSize]      = component;
            mSize++;
        }

        void deleteComponent(const Entity &entity){
            if (mEntityToIndexMap.find(entity) == mEntityToIndexMap.end()) return; // not found, skip
            // have to swap last component to the deleted pos
            // assert(mEntityToIndexMap.find(entity)!=mEntityToIndexMap.end() && "Entity's component doesn't exist");
            size_t indexRemovedComponent = mEntityToIndexMap[entity];
            mComponentArray[indexRemovedComponent] = mComponentArray[mSize - 1];

            Entity lastEntity = mIndexToEntityMap[mSize - 1];
            mEntityToIndexMap[lastEntity] = indexRemovedComponent;
            mIndexToEntityMap[indexRemovedComponent] = lastEntity;

            mEntityToIndexMap.erase(entity);
            mIndexToEntityMap.erase(mSize -1);

            mSize--;
        }

        T& getComponent(const Entity &entity){
            assert(mEntityToIndexMap.find(entity)!=mEntityToIndexMap.end() && "Entity's component doesn't exist");
            return mComponentArray[mEntityToIndexMap[entity]];
        }
};


class ComponentManager{
    private:
    std::unordered_map<const char *,ComponentType> mComponentTypes{};
    std::unordered_map<const char *,std::shared_ptr<IComponentArray>> mComponentArrays{}; // shared pointer for downcasting, cant use unique_ptr
    ComponentType mNextComponentType{};
    
    template<typename T> 
    std::shared_ptr<ComponentArray<T>> getComponentArray(){
        const char *typeName = typeid(T).name();
        assert(mComponentArrays.find(typeName)!=mComponentArrays.end() && "Invalid component name.");
        // have to downcast
        return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
    }

    public:

    template<typename T>
    void createComponentType(){
        const char * typeName = typeid(T).name();
        assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Component Type already exists.");
        mComponentTypes.insert({typeName, mNextComponentType});
        mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
        mNextComponentType++;
    }

    template<typename T>
    ComponentType getComponentType(){
        const char *typeName  = typeid(T).name();
        assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component Type non-existent");
        return mComponentTypes[typeName];
    }

    template<typename T>
    void addComponent(const Entity &entity, const T &component){
        getComponentArray<T>()->insertComponent(entity, component);
    }
    template<typename T>
    void removeComponent(const Entity &entity){
        getComponentArray<T>()->deleteComponent(entity);
    }
    template<typename T>
    T &getComponent(const Entity &entity){
        return getComponentArray<T>()->getComponent(entity);
    }

    void entityDestroyed(Entity entity)
	{
        // destroy all components of the entity
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;
			component->deleteComponent(entity);
		}
	}

};


// COMPONENTS

struct Gravity{
    float value; 
};

struct Ball{
    glm::float32 radius;
};

struct Square{
    GLfloat side;
};

struct Platform{
    GLfloat smallSide;
    GLfloat bigSide;
};

struct Position{
    glm::vec2 position;
};

struct RigidBody{
    glm::vec2 velocity;
    glm::vec2 acceleration;
};

struct Collider{}; //shell component just for checks

struct Renderable {
    Shader* shader;
    GLuint VAO{};
    GLuint VBO{};
    GLuint EBO{};
    glm::vec4 color;
    glm::mat4 modelMatrix{1.0f};
    GLfloat indexCount{}; // how many edges to make the object (ex. square = 6, 2 triangles)

};

struct PlayerInput {
    GLuint leftKey = GLFW_KEY_LEFT;
    GLuint rightKey = GLFW_KEY_RIGHT;
    GLuint upKey = GLFW_KEY_UP;
    GLuint downKey = GLFW_KEY_DOWN;
    GLuint escKey = GLFW_KEY_ESCAPE;
    GLuint bKey = GLFW_KEY_B;
};

struct Text {
    std::string content;
    glm::vec4 color;
    GLfloat scale;
};