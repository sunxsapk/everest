#include "towerdefense.h"
#include "sandbox2d.h"

class TowerDefenseApp : public Application {
    public:
        TowerDefenseApp(){
            pushLayer(new SandBox2D("SandBox2d"));
            //pushLayer(new TowerDefense("Tower defense"));
        }
};

int main(){
    TowerDefenseApp app;
    app.run();
    return 0;
}
