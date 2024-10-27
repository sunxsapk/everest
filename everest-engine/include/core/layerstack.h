/*
 * ======== layer-stack =========
 * Author : Sunil Sapkota
 * Description : Layer stack is the stack-structure that holds the layers in
 * the application. It provides functionality for adding, removing, etc types of
 * operations to the layer in the stack. It is just a container that holds all
 * the layer in the application and provides some abstracted functionality.
 */

#pragma once

#include "layer.h"

namespace Everest {
    typedef std::vector<Layer*> layer_stk;

    class LayerStack {
        public:
            LayerStack();
            ~LayerStack();

            void pushLayer(Layer *layer);
            void popLayer(Layer *layer);
            void popLayer();

            layer_stk::iterator begin(){return _layerStack.begin();}
            layer_stk::iterator end(){return _layerStack.end();}

            layer_stk::reverse_iterator rbegin(){return _layerStack.rbegin();}
            layer_stk::reverse_iterator rend(){return _layerStack.rend();}
        private:
            layer_stk _layerStack;
    };
}
