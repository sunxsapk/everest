#include "towerdefense.h"
#include "sandbox2d.h"

class TowerDefenseApp : public Application {
    public:
        TowerDefenseApp(){
            EV_profile_function();
            pushLayer(new SandBox2D("SandBox2d"));
            //pushLayer(new TowerDefense("Tower defense"));
        }
};

int main(){
    EV_profile_begin("Everest Begin", "profile-res/result_begin.json");
    TowerDefenseApp *app = new TowerDefenseApp;
    EV_profile_end();

    EV_profile_begin("Everest Run", "profile-res/result_run.json");
    app->run();
    EV_profile_end();

    EV_profile_begin("Everest End", "profile-res/result_end.json");
    delete app;
    EV_profile_end();

    return 0;
}
