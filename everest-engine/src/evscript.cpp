#include "scripting/evscript.h"
#include "utils/assetsManager.h"

namespace Everest {
namespace Scripting {

    evscript_c::evscript_c(){
    }

    evscript_c::evscript_c(Entity entity_){
        entity = entity_;
    }

    evscript_c::evscript_c(const evscript_c& other){
        *this = other;
    }

    evscript_c& evscript_c::operator=(const evscript_c& other){
        if(this == &other) return *this;
        scriptpath = other.scriptpath;
        onUpdate = nullptr;
        onCreate = nullptr;
        onCollision = nullptr;
        _initialized = false;
        return *this;
    }

    void evscript_c::init() {
        if(!entity.isValid()) return; // TODO: after fixing above
        if( !scriptpath.has_extension() ||
            AssetsManager::getAssetsType(scriptpath) != AssetsType::SCRIPT
        ) return;

        state = luastate_t();
        state.open_libraries(lualibs::base, lualibs::math);
        registerTypes(state);

        auto lres = state.load_file(scriptpath.c_str());
        if(!lres.valid()){
            throw std::invalid_argument("Unable to load lua script");
        }
        lres();
        // state["entity"] = &entity;

        sol::protected_function _tmp = state["OnCreate"];
        if(_tmp.valid()) {
            onCreate = _tmp;
        }
        _tmp = state["OnUpdate"];
        if(_tmp.valid()){
            onUpdate = _tmp;
        }
        _tmp = state["OnCollision"];
        if(_tmp.valid()){
            onCollision = _tmp;
        }
    }

    void evscript_c::update(double deltaTime){
        if(!_initialized){
            _initialized = true;
            EVLog_Msg("Initialized script %s", scriptpath.c_str());
            if(onCreate) {
                onCreate();
            }
        }

        if(onUpdate){
            onUpdate(deltaTime);
        }
    }
}
}
