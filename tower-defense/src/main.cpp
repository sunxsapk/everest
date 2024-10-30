#include "towerdefense.h"

class TowerDefenseApp : public Application {
    public:
        TowerDefenseApp(){
            pushLayer(new TowerDefense("Tower defense"));
        }
};

int main(){
    TowerDefenseApp app;
    app.run();
    return 0;
}
