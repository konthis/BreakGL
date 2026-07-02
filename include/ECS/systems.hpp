#pragma once
#include "system_manager.hpp"
#include "ECSorganizer.hpp"
#include "components.hpp"
#include "shader.hpp"


extern ECSOrganizer ecs_org;

class PhysicsSystem: public System{
    private:
    public:
    
        void init(){
            for (auto const& entity : mEntities) {
                Signature sig = ecs_org.getSignature(entity);
                auto& grav = ecs_org.getComponent<Gravity>(entity);
                auto& rb = ecs_org.getComponent<RigidBody>(entity);
                rb.acceleration.y = -grav.value;
            }
        }

        void update(float dt){
            for (auto const& entity : mEntities) {
                Signature sig = ecs_org.getSignature(entity);
                auto& rb = ecs_org.getComponent<RigidBody>(entity);
                auto& pos = ecs_org.getComponent<Position>(entity);

                rb.velocity += rb.acceleration*dt;
                pos.position += rb.velocity*dt;
            }
        }
};

class MeshGenSystem: public System{
    private:
    public:
    void init(){
        for (auto const& entity : mEntities) {
            Signature sig = ecs_org.getSignature(entity);
            Position& pos = ecs_org.getComponent<Position>(entity);
            Renderable& rend = ecs_org.getComponent<Renderable>(entity);

            GLuint VAO{};
	        GLuint VBO{};
	        GLuint EBO{};
            std::vector<glm::vec3> vertices;
            std::vector<unsigned int> indices;

            if(sig.test(ecs_org.getComponentType<Square>())){
                Square sq = ecs_org.getComponent<Square>(entity);
                vertices = {
                    glm::vec3{-sq.side/2.,sq.side/2.,0.},
                    glm::vec3{sq.side/2.,sq.side/2.,0.},
                    glm::vec3{sq.side/2.,-sq.side/2.,0.},
                    glm::vec3{-sq.side/2.,-sq.side/2.,0.},
                };
                indices = {0, 1, 2, 0, 2, 3};
            }
            else if(sig.test(ecs_org.getComponentType<Ball>())){
                // make it like a fan, think about it like this
                Ball ball = ecs_org.getComponent<Ball>(entity);
                unsigned int segments = 36;
                for (int i = 0; i <= segments; i++) {
                    float angle = (float)i / segments * 2.f * M_PI;
                    vertices.push_back({
                        ball.radius * cos(angle),
                        ball.radius * sin(angle),
                        0.f
                    });
                }
                // make the fan part, center to top left to top right
                for (int i = 1; i <= segments; i++) {
                    indices.push_back(0);
                    indices.push_back(i);
                    indices.push_back(i + 1);
                }
            }

            glGenVertexArrays(1,&VAO);
            glGenBuffers(1,&VBO);
            glGenBuffers(1,&EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), indices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)nullptr);glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0); 

            rend.EBO = EBO; rend.VAO = VAO; rend.VBO = VBO, rend.indexCount = indices.size();

        }
    }
};

class RenderSystem: public System{
    private:
        glm::mat4 mProjection;
    public:
        void init(GLuint width, GLuint height){
            glClearColor(0.f,0.f,0.f,1.0f);
            mProjection = glm::ortho(0.0f,(float)width,0.0f,(float)height,-1.0f,1.0f);
        }

        void update(){
            glClear(GL_COLOR_BUFFER_BIT);

            for (auto const& entity : mEntities){

                auto& rend = ecs_org.getComponent<Renderable>(entity);
                auto& pos = ecs_org.getComponent<Position>(entity);
                rend.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.position, 0.0f));

                rend.shader->activate();
                rend.shader->setUniform<glm::mat4>("uModel",rend.modelMatrix);
                rend.shader->setUniform<glm::mat4>("uProjection",mProjection);
                rend.shader->setUniform<glm::vec4>("uColor",rend.color);
                rend.shader->setUniform<float>("uTime",(float)glfwGetTime());

                glBindVertexArray(rend.VAO);
                glDrawElements(GL_TRIANGLES, rend.indexCount, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
};

class CollisionSystem: public System{

    private:
        glm::vec4 mWorldDims;
    public:

    void init(glm::vec4 worldDims){
        // x min,xmax,ymin,ymax
        mWorldDims = worldDims;
    }

    void update(){
        for(auto const &entity : mEntities){
            Signature sig = ecs_org.getSignature(entity);
            auto &pos = ecs_org.getComponent<Position>(entity);
            auto &rb = ecs_org.getComponent<RigidBody>(entity);
            glm::vec4 edgeCasesForShape{};
            if(sig.test(ecs_org.getComponentType<Square>())){
                auto &sq = ecs_org.getComponent<Square>(entity);
                float side_seconds = sq.side/2.;
                edgeCasesForShape = {
                    side_seconds, mWorldDims[1] - side_seconds,side_seconds, mWorldDims[3] - side_seconds
                };
            }
            else if(sig.test(ecs_org.getComponentType<Ball>())){
                auto &circle = ecs_org.getComponent<Ball>(entity);
                float radius_seconds = circle.radius/2.;
                edgeCasesForShape = {
                    radius_seconds, mWorldDims[1] - radius_seconds,radius_seconds, mWorldDims[3] - radius_seconds
                };

            }

            if (pos.position.y <= edgeCasesForShape[2]) {
                pos.position.y = edgeCasesForShape[2];
                rb.velocity.y = glm::abs(rb.velocity.y);
            }
            if (pos.position.y >= edgeCasesForShape[3]) {
                pos.position.y = edgeCasesForShape[3];
                rb.velocity.y = -glm::abs(rb.velocity.y);
            }
            if (pos.position.x <= edgeCasesForShape[0]) {
                pos.position.x = edgeCasesForShape[0];
                rb.velocity.x = glm::abs(rb.velocity.x); 
            }
            if (pos.position.x >= edgeCasesForShape[1]) {
                pos.position.x = edgeCasesForShape[1];
                rb.velocity.x = -glm::abs(rb.velocity.x);
            }

            
        }
    }
};

class InputSystem: public System{
    private:
    public:
    void init(){}
    void update(Window &window) {
        for (auto const& entity : mEntities) {
            auto& gravity = ecs_org.getComponent<Gravity>(entity);
            auto& pos = ecs_org.getComponent<Position>(entity);
            auto& input = ecs_org.getComponent<PlayerInput>(entity);

            if(window.processKeyPress() == input.upKey){
                // gravity.value = gravity.value > 0 ? -gravity.value:gravity.value;

                pos.position.y += 1.0f;
            }
            if(window.processKeyPress() == input.downKey){
                // gravity.value = gravity.value > 0 ? gravity.value:-gravity.value;
                pos.position.y -= 1.0f;
            }
            if(window.processKeyPress() == input.rightKey){
                pos.position.x += 1.0f;
            }
            if(window.processKeyPress() == input.leftKey){
                pos.position.x -= 1.0f;
            }
        }
    }
};