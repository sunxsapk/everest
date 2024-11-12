#include "towerdefense.h"
#include "sandbox2d.h"
#include "docktest.h"

class TowerDefenseApp : public Application {
    public:
        TowerDefenseApp(){
            EV_profile_function();
            pushLayer(new SandBox2D("SandBox2d"));
            //pushLayer(new DockLayer("Dock"));
            //pushLayer(new TowerDefense("Tower defense"));
        }
};

Application* Everest::createApplication(Everest::CommandLineArgs args){
    return new TowerDefenseApp();
}

