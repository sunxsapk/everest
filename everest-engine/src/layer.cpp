#include "core/layer.h"

namespace Everest{
    Layer::Layer(const char* name) : _name(name){
    }

    Layer::~Layer(){
    }

    void Layer::onAttach(){}
    void Layer::onDetach(){}

    void Layer::onEvent(Event& event){}
    void Layer::onUpdate(){}

    void Layer::onDebugRender(){}
}
