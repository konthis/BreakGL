#include "entities.hpp"
#include "components.hpp"


int main(){
    EntityManager em;
    Entity e = em.createEntity();
    Signature s{};
    em.setSignature(e,s);

    ComponentManager cm;
    Position t = {5.,5.};
    Circle c = {0.1, {1.,0.,0.,1.}};
    cm.createComponentType<Position>();
    cm.createComponentType<Circle>();
    
    cm.addComponent(e,t);
    cm.addComponent(e,c);

    
    
    em.printEntity(e);
    s.set(cm.getComponentType<Position>());
    s.set(cm.getComponentType<Circle>());
    em.printEntity(e);
    
    return 0;
}