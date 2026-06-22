#include "entities.hpp"
#include "components.hpp"


int main(){
    EntityManager em;
    Entity e = em.createEntity();
    em.printEntity(e);
    Signature s{};
    s.set(0);s.set(10);
    em.setSignature(e,s);
    em.printEntity(e);

    ComponentManager cm;
    Translate t;
    t.print();
    cm.createComponentType<Translate>();
    
    return 0;
}