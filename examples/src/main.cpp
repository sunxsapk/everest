#include <everest.h>


using namespace Everest;

class PoolGameLayer : public Layer {
    public:
        PoolGameLayer(const char* name) : Layer(name){ }

        void onAttach() override {
            SceneManager::loadScene("assets/scenes/pool_game.everest");
        }

        void onUpdate() override {
            Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
            Renderer::issue_clear();

            auto& activeScene = SceneManager::getActiveScene();
            if(activeScene){
                activeScene->onUpdate();
                activeScene->onRender();
            }
        }

        void onEvent(Event& event) override{
            EventDispatcher dispatcher(event);

            dispatcher.dispatch<WindowResizeEvent>([](WindowResizeEvent& event)->bool{
                    SceneManager::getActiveScene()->onViewportResize(vec2(), event.getSize());
                    return true;
                });

        }
};

class PoolGameApp : public Application {
    public:
        PoolGameApp(){
            EV_profile_function();
            pushLayer(new PoolGameLayer("SandBox2d"));
        }
};

Application* Everest::createApplication(Everest::CommandLineArgs args){
    return new PoolGameApp();
}

