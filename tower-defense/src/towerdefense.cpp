#include "towerdefense.h"


TowerDefense::TowerDefense(const char* name)
    :Everest::Layer(name){
}

void TowerDefense::onUpdate(){
    if(Everest::Input::getKeyDown(Everest::K_q)){
        Everest::Debug::Warn("jhyakneyy");
    }

    Everest::vec2 scroll = Everest::Input::mouseScroll();
    if(scroll.x || scroll.y){
        Everest::Debug::Warn("scroll : %f, %f", scroll.x, scroll.y);
    }
}

void TowerDefense::onDebugRender(){
}
