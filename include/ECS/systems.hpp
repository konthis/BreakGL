#pragma once
#include "system_manager.hpp"
#include "ECSorganizer.hpp"
#include "components.hpp"
#include "shader.hpp"
#include "text.hpp"
#include "constants.hpp"
#include "prefabs.hpp"


extern ECSOrganizer ecs_org;

class PhysicsSystem: public System{
    private:
    public:
    
        void init(){

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

    void initEntity(Entity entity){
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

        else if(sig.test(ecs_org.getComponentType<Platform>())){
            Platform pl = ecs_org.getComponent<Platform>(entity);
            vertices = {
                glm::vec3{-pl.bigSide/2.,pl.smallSide/2.,0.},
                glm::vec3{pl.bigSide/2.,pl.smallSide/2.,0.},
                glm::vec3{pl.bigSide/2.,-pl.smallSide/2.,0.},
                glm::vec3{-pl.bigSide/2.,-pl.smallSide/2.,0.},
            };

            indices = {0, 1, 2, 0, 2, 3};
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

    void init(){
        for (auto const& entity : mEntities) {
            initEntity(entity);
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
                if(rend.hidden){ continue;}

                auto& pos = ecs_org.getComponent<Position>(entity);
                rend.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.position, 0.0f));

                rend.shader->activate();

                Signature sig = ecs_org.getSignature(entity);
                if (sig.test(ecs_org.getComponentType<Ball>())){
                    rend.shader->setUniform<float>("uRadius",BALL_RADIUS);
                }

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
        void checkWallCollisions(Position &pos, RigidBody &rb,glm::vec4 &edgeCasesForShape, bool isPlatform){
            if (pos.position.y <= edgeCasesForShape[2] && isPlatform) {
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

        void checkEntityCollisions(Entity e1, Entity e2, std::set<Entity>& toDestroy, unsigned int &toCreate, bool &widePlatform){
            auto& posE1 = ecs_org.getComponent<Position>(e1);
            auto &rbE1 = ecs_org.getComponent<RigidBody>(e1);
            auto& posE2 = ecs_org.getComponent<Position>(e2);
            auto &rbE2 = ecs_org.getComponent<RigidBody>(e2);
            Signature sig1 = ecs_org.getSignature(e1);
            Signature sig2 = ecs_org.getSignature(e2);

            glm::vec2 delta;
            float distance, minDistance;
            // ball x ball collision
            if(sig1.test(ecs_org.getComponentType<Ball>()) &&
                sig2.test(ecs_org.getComponentType<Ball>())){
                
                float radE1 = ecs_org.getComponent<Ball>(e1).radius;
                float radE2 = ecs_org.getComponent<Ball>(e2).radius;
                
                delta = posE2.position - posE1.position; 
                distance = glm::length(delta);
                minDistance = radE1 + radE2;

                if(distance < minDistance && distance > 0.f){
                    glm::vec2 n = delta / distance; //normal vector
                    
                    // push them
                    float overlap = minDistance - distance;
                    posE1.position -= n * overlap * 0.5f;
                    posE2.position += n * overlap * 0.5f;

                    // THIS COMMEND PART IS FOR NORMAL VELOCITY CHANGE, IM BREAKOUT, WE HAVE FIXED VELOCITIES
                    float speed1 = glm::length(rbE1.velocity);
                    float speed2 = glm::length(rbE2.velocity);
                    // update velocities on the norm
                    float dvn = glm::dot(rbE1.velocity - rbE2.velocity, n);
                    if (dvn > 0.f) {  // only resolve if moving toward each other
                        rbE1.velocity -= dvn * n;
                        rbE2.velocity += dvn * n;
                        if(glm::length(rbE1.velocity) > 0.f) rbE1.velocity = glm::normalize(rbE1.velocity) * speed1;
                        if(glm::length(rbE2.velocity) > 0.f) rbE2.velocity = glm::normalize(rbE2.velocity) * speed2;

                    }
                }
            }

            // platform x ball collision
            else if( 
                sig2.test(ecs_org.getComponentType<Platform>()) &&
                sig1.test(ecs_org.getComponentType<Ball>())){
                
                Entity plE = e2, ballE=e1;

                const auto& pl = ecs_org.getComponent<Platform>(plE);
                const auto& plPosition = ecs_org.getComponent<Position>(plE);
                auto &plRB = ecs_org.getComponent<RigidBody>(plE);
                float rad = ecs_org.getComponent<Ball>(ballE).radius;
                auto &ballPosition = ecs_org.getComponent<Position>(ballE);
                auto &ballRB = ecs_org.getComponent<RigidBody>(ballE);

                glm::vec2 halfExt = { pl.bigSide/2.f, pl.smallSide/2.f };
                delta = ballPosition.position - plPosition.position; 
                // to find the contact point
                glm::vec2 closest = glm::clamp(delta, -halfExt, halfExt);

                glm::vec2 contactPoint = plPosition.position + closest;
                glm::vec2 diff = ballPosition.position - contactPoint;
                distance = glm::length(diff); 

                // relative velocity, to correctly calculate, when platform is moving, slightly decreased for smooth gameplay
                glm::vec2 relVel = ballRB.velocity - plRB.velocity/4.f;

                if (distance < rad && distance > 0.f) {
                    glm::vec2 normal = glm::normalize(diff);
                    ballPosition.position += normal * (rad - distance);
                
                    float dvn = glm::dot(relVel, normal);
                    if (dvn < 0.f) {
                        if (normal.y > 0.5f) {  // top of the platform
                            // make the platform be different from walls on reflection
                            // slightly left of the platform contact->ball goes slightly left too
                            float offset = (ballPosition.position.x - plPosition.position.x) / (pl.bigSide / 2.f);
                            float speed = glm::length(ballRB.velocity);
                            ballRB.velocity.x = speed * glm::clamp(offset, -1.f, 1.f);
                            ballRB.velocity.y = glm::abs(ballRB.velocity.y);
                            ballRB.velocity = glm::normalize(ballRB.velocity) * speed;
                        } else {  // sides pure reflection
                            ballRB.velocity -= 2.f * dvn * normal;
                        }
                    }
                }


            }

            // ball x square collision
            else if(
                sig2.test(ecs_org.getComponentType<Square>()) &&
                sig1.test(ecs_org.getComponentType<Ball>())){

                Entity sqE = e2,ballE = e1;

                const auto& sq = ecs_org.getComponent<Square>(sqE);
                const auto& sqPosition = ecs_org.getComponent<Position>(sqE);
                float rad = ecs_org.getComponent<Ball>(ballE).radius;
                auto &ballPosition = ecs_org.getComponent<Position>(ballE);
                auto &ballRB = ecs_org.getComponent<RigidBody>(ballE);
                
                glm::vec2 halfExt = { sq.side/2.f, sq.side/2.f};
                delta = ballPosition.position - sqPosition.position; 
                // to find the contact point
                glm::vec2 closest = glm::clamp(delta, -halfExt, halfExt);

                glm::vec2 contactPoint = sqPosition.position + closest;
                glm::vec2 diff = ballPosition.position - contactPoint;
                distance = glm::length(diff); 

                if (distance <= rad && distance > 0.f) { // if collision
                    glm::vec2 normal = glm::normalize(diff);
                    ballPosition.position += normal * (rad - distance);
                
                    float dvn = glm::dot(ballRB.velocity, normal);
                    if (dvn < 0.f){
                        ballRB.velocity -= 2.f * dvn * normal;
                    }
                    toDestroy.insert(sqE);

                    if(sq.power == PowerUp::SUMMON_BALL){
                        // JUST SUMMON ANOTHER BALL FOR NOW
                        toCreate++;
                        //
                    }
                    if(sq.power == PowerUp::LONGER_PLATFORM){
                        widePlatform = true;
                    }
                }
            }
        }

    public:

    void init(glm::vec4 worldDims){
        // x min,xmax,ymin,ymax
        mWorldDims = worldDims;
    }

    void update(CollisionResult &cr){
        for (auto const& entity : mEntities) {
            auto const& sig = ecs_org.getSignature(entity);
            // PLATFORM COLLISION (ONLY WITH WALLS NOW)
            if(sig.test(ecs_org.getComponentType<Platform>())){
                auto &plat = ecs_org.getComponent<Platform>(entity);
                auto &pos = ecs_org.getComponent<Position>(entity);
                auto &rb = ecs_org.getComponent<RigidBody>(entity);
                float smallSide_seconds = plat.smallSide/2.;
                float bigSide_seconds = plat.bigSide/2.;
                glm::vec4 edgeCasesForShape = {
                    bigSide_seconds, mWorldDims[1] - bigSide_seconds, smallSide_seconds, mWorldDims[3] -smallSide_seconds 
                };
                checkWallCollisions(pos,rb,edgeCasesForShape,
                    sig.test(ecs_org.getComponentType<Platform>()));
            }
            // BALL COLLISIONS
            else if(sig.test(ecs_org.getComponentType<Ball>())){
                // set because sometimes frames add the same squares
                auto &ball = ecs_org.getComponent<Ball>(entity);
                auto &pos = ecs_org.getComponent<Position>(entity);
                auto &rb = ecs_org.getComponent<RigidBody>(entity);
                float radius_seconds = BALL_RADIUS/2.;
                if(pos.position.y < - 2.0f*BALL_RADIUS){
                    // delete ball if out of bottom
                    // ecs_org.destroyEntity(entity);
                    cr.toDestroy.insert(entity);
                    return;
                }
                glm::vec4 edgeCasesForShape = {
                    radius_seconds, mWorldDims[1] - radius_seconds,radius_seconds, mWorldDims[3] - radius_seconds
                };

                checkWallCollisions(pos,rb,edgeCasesForShape,
                    sig.test(ecs_org.getComponentType<Platform>()));

                unsigned int toCreateNum = 0;
                for (auto const& entity2 : mEntities){
                    if(entity != entity2){
                        checkEntityCollisions(entity, entity2, cr.toDestroy, toCreateNum,cr.widePlatform);
                        if(toCreateNum) break; // only 1 detroy per frame PER BALL
                    }
                }
                if(toCreateNum){
                    auto &rend = ecs_org.getComponent<Renderable>(entity);

                    for (int i = 0; i < toCreateNum; i++){
                        cr.ballsToSpawn.push_back(BallSpawn{
                            .position = glm::vec2{pos.position.x, WINDOW_HEIGHT/5.0f},
                            .velocity = glm::vec2{-rb.velocity.x, glm::abs(rb.velocity.y)}
                        });
                    }
                }
            }
        }
    }
};

class InputSystem: public System{
    private:
    public:
    void init(){}
    void update(GLuint &key) {
        for (auto const& entity : mEntities) {
            auto& pos = ecs_org.getComponent<Position>(entity);
            auto& rb = ecs_org.getComponent<RigidBody>(entity);
            auto& input = ecs_org.getComponent<PlayerInput>(entity);

            rb.velocity = glm::vec2{};
            float speed = 400.0f;
            if(key == input.upKey){
                // rb.velocity.y = speed;
                // pos.position.y += 1.0f;
            }
            if(key == input.downKey){
                // rb.velocity.y = -speed;
                // pos.position.y -= 1.0f;
            }
            if(key == input.rightKey){
                rb.velocity.x = speed;
                // pos.position.x += 1.0f;
            }
            if(key == input.leftKey){
                rb.velocity.x = -speed;
                // pos.position.x -= 1.0f;
            }
        }
    }
};

class TextRenderSystem: public System{
    private:
        GLuint mVAO{};
	    GLuint mVBO{};
        TextRenderer mTR;
        glm::mat4 mProjection;
        Shader *mTextShader;
    public:
        ~TextRenderSystem() {
            glDeleteVertexArrays(1, &mVAO);
            glDeleteBuffers(1, &mVBO);
        }
        void init(GLuint width, GLuint height, Shader *textShader, unsigned int pixelSize){
            mTR.init(pixelSize);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
            mProjection = glm::ortho(0.0f,(float)width,0.0f,(float)height,-1.0f,1.0f);

            glGenVertexArrays(1, &mVAO);
            glGenBuffers(1, &mVBO);
            glBindVertexArray(mVAO);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            mTextShader = textShader;
        }
        void update(){
            for (auto const& entity : mEntities){
                auto const& text = ecs_org.getComponent<Text>(entity);
                // position is middle left of the text
                auto& pos = ecs_org.getComponent<Position>(entity);
                mTextShader->activate();
                mTextShader->setUniform<glm::vec4>("uTextColor",text.color);
                mTextShader->setUniform<glm::mat4>("uProjection", mProjection);
                glActiveTexture(GL_TEXTURE0);
                glBindVertexArray(mVAO);
                const auto& characters = mTR.getCharacters();
                // iterate through all characters
                std::string::const_iterator c;
                float x = text.centered?pos.position.x-getTextWidth(characters,text.content,text.scale)/2.0f:pos.position.x;
                float y = pos.position.y;
                for (c = text.content.begin(); c != text.content.end(); c++){
                    Character ch = characters.at(*c);
            
                    float xpos = x + ch.bearing.x * text.scale;
                    float ypos = y - (ch.size.y - ch.bearing.y) * text.scale;
            
                    float w = ch.size.x * text.scale;
                    float h = ch.size.y * text.scale;
                    // update VBO for each character
                    float vertices[6][4] = {
                        { xpos,     ypos + h,   0.0f, 0.0f },            
                        { xpos,     ypos,       0.0f, 1.0f },
                        { xpos + w, ypos,       1.0f, 1.0f },
                    
                        { xpos,     ypos + h,   0.0f, 0.0f },
                        { xpos + w, ypos,       1.0f, 1.0f },
                        { xpos + w, ypos + h,   1.0f, 0.0f }           
                    };
                    // render glyph texture over quad
                    glBindTexture(GL_TEXTURE_2D, ch.textureID);
                    // update content of VBO memory
                    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    // render quad
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                    x += (ch.advance >> 6) * text.scale; // bitshift by 6 to get value in pixels (2^6 = 64)
                }
                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
        
};

class MenuInputSystem: public System{
    private:
        GLuint mSelectedIdx = 0;
        GLuint mPrevKey = GLFW_KEY_UNKNOWN; // to change keys on diff keypress
        float mKeyHeldTime = 0.0f;
        const float KEY_REPEAT = 0.15f; // seconds between moves while held

        bool handleKey(GLuint &key, bool horizontal){
            GLuint maxOpts = (GLuint)mEntities.size();
            if(key == GLFW_KEY_UP && !horizontal){
                mSelectedIdx = mSelectedIdx>0?mSelectedIdx-1:0;
            }
            if(key == GLFW_KEY_DOWN && !horizontal){
                mSelectedIdx = mSelectedIdx<maxOpts-1?mSelectedIdx+1:maxOpts- 1;
            }
            if(key == GLFW_KEY_RIGHT && horizontal){
                mSelectedIdx = mSelectedIdx<maxOpts-1?mSelectedIdx+1:maxOpts- 1;
            }
            if(key == GLFW_KEY_LEFT && horizontal){
                mSelectedIdx = mSelectedIdx>0?mSelectedIdx-1:0;
            }
            if(key == GLFW_KEY_ENTER){
                return true;
            }
            for (auto const& entity : mEntities) {
                auto& menuopt = ecs_org.getComponent<MenuOption>(entity);
                auto& text = ecs_org.getComponent<Text>(entity);
                bool isSelected = (menuopt.index == mSelectedIdx);
                menuopt.isSelected = isSelected;
                text.color = isSelected ? MENU_SELECTED_TEXT_COLOR : MENU_TEXT_COLOR;
            }
            return false;
        }
    public:
    void init(){
        for (auto const& entity : mEntities){
            auto& menuopt = ecs_org.getComponent<MenuOption>(entity);
            if(menuopt.isSelected){
                auto& text = ecs_org.getComponent<Text>(entity);
                text.color = MENU_SELECTED_TEXT_COLOR;
            }
        }
    }
    int update(GLuint &key, float dt, bool horizontal) {

        bool enterPressed;
        if (key != mPrevKey) { // new press
            enterPressed = handleKey(key, horizontal);
            mKeyHeldTime = 0.0f;
        } 
        else if (key != GLFW_KEY_UNKNOWN && key != GLFW_KEY_ENTER) {  // held
            mKeyHeldTime += dt;
            if (mKeyHeldTime >= KEY_REPEAT) {
                enterPressed = handleKey(key, horizontal);
                mKeyHeldTime = 0.0f;
            }
        }
        mPrevKey = key;
        // IF ENTER PRESSED, RETURN THE SELECTED IDX
        if(enterPressed){
            return mSelectedIdx;
        }
        // ELSE RETURN THE NEGATIVE - 1, TO CALCULATE HOVERED OPTIONS
        else{
            return -mSelectedIdx-1;
        }
    }

    void reset() {
        mSelectedIdx = 0;
        mKeyHeldTime = 0.0f;
    }



};
