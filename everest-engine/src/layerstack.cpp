#include "core/layerstack.h"

namespace Everest {
    LayerStack::LayerStack(){
    }

    LayerStack::~LayerStack(){
        for(auto& layer:this->_layerStack){
            layer->onDetach();
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer *layer){
        ASSERT(layer != NULL, "Invalid layer");
        this->_layerStack.emplace_back(layer);
    }

    void LayerStack::popLayer(Layer *layer){
        ASSERT(layer != NULL, "Invalid layer");
        auto itr = std::find(_layerStack.begin(), _layerStack.end(), layer);
        if(itr != _layerStack.end()){
            layer->onDetach();
            _layerStack.erase(itr);
        }
    }

    void LayerStack::popLayer(){
        Layer* layer = this->_layerStack.back();
        ASSERT(layer != NULL, "Attempt to pop from empty layer stack.");
        layer->onDetach();
        this->_layerStack.pop_back();
    }
}
