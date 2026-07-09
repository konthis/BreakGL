#include "prefabs.hpp"

Entity createBall(ECSOrganizer& ecs, Shader* ballShader, glm::vec2 pos, glm::vec2 vel){
    Entity e = ecs.createEntity();
    ecs.addComponent<Ball>(e, Ball{ 
        .radius = BALL_RADIUS}
    );
    ecs.addComponent<Position>(e, Position{ 
        .position = pos
    });
    ecs.addComponent<RigidBody>(e, RigidBody{ 
        .velocity = vel}
    );
    ecs.addComponent<Collider>(e,Collider{});
    ecs.addComponent<Renderable>(e, Renderable{ 
        .shader = ballShader, 
        .color = COLOR_WHITE 
    });
    return e;
}

Entity createPlatform(ECSOrganizer& ecs, Shader* platformShader,
glm::vec2 pos){
    Entity e = ecs.createEntity();
    ecs.addComponent<Platform>(e, Platform{
        .smallSide = PLATFORM_SIDE_SMALL,
        .bigSide = PLATFORM_SIDE_BIG
    });
    ecs.addComponent<Position>(e, Position{ 
        .position = pos
    });
    ecs.addComponent<RigidBody>(e, RigidBody{ 
        .velocity = glm::vec2{0.0f,0.0f}
    });
    ecs.addComponent<Collider>(e,Collider{});
    ecs.addComponent<Renderable>(e, Renderable{ 
        .shader = platformShader, 
        .color = COLOR_WHITE
    });
    ecs.addComponent<PlayerInput>(e,PlayerInput{});
    return e;
}

Entity createText(ECSOrganizer& ecs, const std::string content, glm::vec4 color, float scale, glm::vec2 pos){
    Entity e = ecs.createEntity();
    ecs.addComponent<Text>(e,Text{
        .content = content,
        .color = color,
        .scale = scale
    });
    ecs.addComponent<Position>(e, Position{
        .position = pos
    });

    return e;
}

Entity createSquare(ECSOrganizer& ecs, Shader* squareShader, glm::vec2 pos, PowerUp power){
    Entity e = ecs.createEntity();
    ecs.addComponent<Position>(e, Position{ 
        .position = pos
    });
    ecs.addComponent<RigidBody>(e, RigidBody{
        .velocity = glm::vec2{0.0f,0.0f},
        .acceleration = glm::vec2{0.0f,0.0f}
    });
    ecs.addComponent<Collider>(e,Collider{});

    ecs.addComponent<Square>(e, Square{ 
        .side = SQUARE_SIDE,
        .power = power
    });
    if(power == PowerUp::SUMMON_BALL){
        ecs.addComponent<Renderable>(e, Renderable{ squareShader, 
            .color = SQUARE_COLOR_YELLOW 
        });

    }
    else if(power == PowerUp::LONGER_PLATFORM){
        ecs.addComponent<Renderable>(e, Renderable{ squareShader, 
            .color = SQUARE_COLOR_RED 
        });

    }
    else{
        ecs.addComponent<Renderable>(e, Renderable{ squareShader, 
            .color = SQUARE_COLOR_RED 
        });

    }

    return e;
}

Entity createSquareHollow(ECSOrganizer& ecs, Shader* squareShader, glm::vec2 pos, PowerUp power, const GLuint sceneIdx){
    Entity e = ecs.createEntity();
    ecs.addComponent<Position>(e, Position{ 
        .position = pos
    });
    ecs.addComponent<Square>(e, Square{ 
        .side = SQUARE_SIDE,
        .power = power
    });
    ecs.addComponent<Preview>(e, Preview{
        .sceneIdx = sceneIdx
    });

    if(power == PowerUp::SUMMON_BALL){
        ecs.addComponent<Renderable>(e, Renderable{ squareShader, 
            .color = SQUARE_COLOR_YELLOW,
            .hidden = true 
        });

    }
    else if(power == PowerUp::LONGER_PLATFORM){
        ecs.addComponent<Renderable>(e, Renderable{ squareShader, 
            .color = SQUARE_COLOR_RED,
            .hidden = true 
        });

    }
    else{
        ecs.addComponent<Renderable>(e, Renderable{ squareShader, 
            .color = SQUARE_COLOR_RED, 
            .hidden = true 
        });

    }

    return e;
}