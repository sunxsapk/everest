#include "towerdefense.h"

TowerDefense::TowerDefense(const char* name)
    :Layer(name){
}

void TowerDefense::onUpdate(){
    if(Input::getKeyDown(K_q)){
        Debug::Warn("jhyakneyy");
    }

    /*
    vec2 scroll = Input::mouseScroll();
    if(scroll.x || scroll.y){
        Debug::Warn("scroll : %f, %f", scroll.x, scroll.y);
    }
    */
}

void TowerDefense::onDebugRender(){
    ImGui::Begin(this->_name);
    ImGui::Text("%s", _evLog);
    ImGui::End();
}

void TowerDefense::onEvent(Event& event){
    sprintf(this->_evLog, "%s", event.toString().c_str());
}
