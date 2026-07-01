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
            
        }
        void update(float dt){
            for (auto const& entity : mEntities) {
                Signature sig = ecs_org.getSignature(entity);
                auto& grav = ecs_org.getComponent<Gravity>(entity);
                auto& pos = ecs_org.getComponent<Position>(entity);

                // gravity
                pos.position.y -=grav.g*dt;
                // THIS WILL PROLLY GO TO COLLISION CHECK SYSTEM
                if(pos.position.y<=0){ pos.position.y = 0;}
                else if(pos.position.y>=600){ pos.position.y = 600;}
                if(pos.position.x<=0){ pos.position.x = 0;}
                else if(pos.position.x>=800){ pos.position.x = 800;}
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

            if(sig.test(ecs_org.getComponentType<Square>())){
                Square sq = ecs_org.getComponent<Square>(entity);
                std::vector<glm::vec3> vertices = {
                    glm::vec3{-sq.side/2.,sq.side/2.,0.},
                    glm::vec3{sq.side/2.,sq.side/2.,0.},
                    glm::vec3{sq.side/2.,-sq.side/2.,0.},
                    glm::vec3{-sq.side/2.,-sq.side/2.,0.},
                };
                unsigned int indices[] = {
                    0, 1, 2, 0, 2, 3  
                };
                
                glGenVertexArrays(1,&VAO);
                glGenBuffers(1,&VBO);
                glGenBuffers(1,&EBO);

                glBindVertexArray(VAO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)nullptr);glEnableVertexAttribArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0); 
                glBindVertexArray(0); 

                rend.EBO = EBO; rend.VAO = VAO; rend.VBO = VBO;


            }
            else{// BALL ETC.
            }




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
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
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
                gravity.g = gravity.g > 0 ? -gravity.g:gravity.g;
            }
            if(window.processKeyPress() == input.downKey){
                gravity.g = gravity.g > 0 ? gravity.g:-gravity.g;
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