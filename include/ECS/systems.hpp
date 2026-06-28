#pragma once
#include "system_manager.hpp"
#include "ECSorganizer.hpp"
#include "components.hpp"

extern ECSOrganizer ecs_org;

class PhysicsSystem: public System{
    private:
    public:
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
                    glm::vec3{pos.position.x-sq.side/2.,pos.position.y+sq.side/2.,0.},
                    glm::vec3{pos.position.x+sq.side/2.,pos.position.y+sq.side/2.,0.},
                    glm::vec3{pos.position.x+sq.side/2.,pos.position.y-sq.side/2.,0.},
                    glm::vec3{pos.position.x-sq.side/2.,pos.position.y-sq.side/2.,0.},
                };
                unsigned int indices[] = {
                    0, 1, 2, 0, 2, 3  
                };
                
                glGenVertexArrays(1,&VAO);
                glGenBuffers(1,&VBO);
                glGenBuffers(1, &EBO);

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
    public:
        void init(){
            glClearColor(0.1f,0.1f,0.1f,1.0f);
            

        }

        void update(float dt){
            glClear(GL_COLOR_BUFFER_BIT);

            for (auto const& entity : mEntities){

                // std::cout << glm::to_string(ecs_org.getComponent<Square>(entity).color) <<"\n";
                auto& rend = ecs_org.getComponent<Renderable>(entity);
                rend.shader->activate();
                glBindVertexArray(rend.VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
};