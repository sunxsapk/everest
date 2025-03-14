#include "scripting/evscript.h"
#include "utils/assetsManager.h"
#include "scripting/typeregistry.h"
#include "scene/def_components.h"

namespace Everest {
namespace Scripting {

    scriptHandler_t::scriptHandler_t(const scriptHandler_t& other){
        *this = other;
    }

    scriptHandler_t::scriptHandler_t(Entity ent, std::filesystem::path path)
    :scriptpath(path){
        init(ent);
    }

    scriptHandler_t& scriptHandler_t::operator=(const scriptHandler_t& other){
        if(this == &other) return *this;
        scriptpath = other.scriptpath;
        state = other.state;
        onCreate = other.onCreate;
        onUpdate = other.onUpdate;
        onCollision = other.onCollision;
        _initialized = false;
        return *this;
    }

    void scriptHandler_t::init(Entity entity){
        if(!entity.isValid()) return;
        if(!std::filesystem::exists(scriptpath)) return;
        if( !scriptpath.has_extension() ||
            AssetsManager::getAssetsType(scriptpath) != AssetsType::SCRIPT
        ) return;

        state = createRef<luastate_t>();
        auto& __state = *state;
        __state.open_libraries(lualibs::base, lualibs::math);
        registerTypes(__state);
        auto lres = __state.load_file(scriptpath.string().c_str());
        if(!lres.valid()){
            throw std::invalid_argument("Unable to load lua script");
        }
        lres();
        __state["entity"] = entity;
        if(__state["OnCreate"].valid()) {
            onCreate = __state["OnCreate"];
        }
        if(__state["OnUpdate"].valid()){
            onUpdate = __state["OnUpdate"];
        }
        if(__state["OnCollision"].valid()){
            onCollision = __state["OnCollision"];
        }
        EVLog_Msg("Parsed script %s with serialization validity(%d)", scriptpath.c_str(), __state["SERIALIZE"].valid());
    }

    bool scriptHandler_t::getSerializedFields(sol::table& resultTable) const {
        if(state == nullptr || (*state)["SERIALIZE"].valid()){
            resultTable = (*state)["SERIALIZE"];
            return true;
        }
        return false;
    }

    void scriptHandler_t::update(double deltaTime){
        if(!_initialized){
            _initialized = true;
            if(onCreate) {
                onCreate();
            }
        }

        if(onUpdate){
            onUpdate(deltaTime);
        }
    }

    void scriptHandler_t::collisionCallback(collision2d_t& data){
        if(onCollision){
            onCollision(data);
        }
    }

    void scriptHandler_t::setScriptPath(std::filesystem::path path, Entity ent){
        if(scriptpath == path) return;
        scriptpath = path;
        init(ent);
    }

    std::string scriptHandler_t::getScriptName(){
        return scriptpath.stem().string();
    }

    sol::table scriptHandler_t::call(std::string func_name, sol::table inp_args) {
        if(!state) return {};
        sol::protected_function func = (*state)[func_name];
        if(!func.valid()) return {};

        try {
            sol::table iargs = state->create_table();
            for(auto& [k, v] : inp_args){
                if(v.get_type() == sol::type::userdata){
                    EVLog_Err("Unsupported action: Passing userdata as arguments across scripts.");
                    iargs[k.as<const char*>()] = nullptr; // nil
                } else {
                    iargs[k.as<const char*>()] = v;
                }
            }
            return func(iargs);
        } catch(std::exception exc) {
            EVLog_Err("Exception occured while calling function %s in script %s: %s",
                    func_name.c_str(), getScriptName().c_str(), exc.what());
        }

        return {};
    }

    evscript_c::evscript_c(const evscript_c& other) {
        *this = other;
    }

    evscript_c& evscript_c::operator=(const evscript_c& other){
        if(this == &other) return *this;
        entity = other.entity;
        scripts = other.scripts;
        return *this;
    }

    void evscript_c::makeCopyUsing(const evscript_c& other, Entity entity_){
        entity = entity_;
        this->scripts.clear();
        for(auto& script : other.scripts){

            this->scripts.push_back(scriptHandler_t());
            auto& lastScript = this->scripts.back();
            lastScript.scriptpath = script.scriptpath;
            lastScript.init(entity);

            auto& _sstate = *script.state;
            auto& _lstate = *lastScript.state;
            if(!_sstate["SERIALIZE"].valid()) continue;
            sol::table st = _sstate["SERIALIZE"];
            for(auto& [k, v] : st){

                const char* key = k.as<const char*>();

                if(!v.valid()) continue;
                if(!_sstate[key].valid()) continue;
                // if(!_lstate[key].valid()) continue;

                switch(v.as<Types>()){
                    case Types::Int: 
                        {
                            int v = _sstate[key];
                            _lstate[key] = v;
                            break;
                        }
                    case Types::Bool:
                        {
                            bool v = _sstate[key];
                            _lstate[key] = v;
                            break;
                        }
                    case Types::Float:
                        {
                            float v = _sstate[key];
                            _lstate[key] = v;
                            break;
                        }
                    case Types::String:
                        {
                            std::string v = _sstate[key];
                            _lstate[key] = v;
                            break;
                        }
                    case Types::Vec2:
                        {
                            vec2 v = _sstate[key];
                            _lstate[key] = v;
                            break;
                        }
                    case Types::Vec3: 
                        {
                            vec3 v = _sstate[key];
                            _lstate[key] = v;
                            break;
                        }
                    case Types::Vec4: 
                        {
                            vec4 v = _sstate[key];
                            _lstate[key] = v;
                            break;
                        }
                    case Types::Color: 
                        {
                            vec4 v = _sstate[key];
                            _lstate[key] = v;
                            break;
                        }
                    case Types::Entity:
                        {
                            Entity se = _sstate[key];
                            if(!se.isValid()) continue;
                            _lstate[key] = entity.getScene()->getEntityFromId(se.get<id_c>().id);
                            break;
                        }
                    default:
                        EVLog_Wrn("unsupported type");
                        break;
                }

            }
        }
    }

    void evscript_c::init() {
        for(auto& script : scripts){
            script.init(entity);
        }
    }

    void evscript_c::update(double deltaTime){
        for(auto& script : scripts){
            script.update(deltaTime);
        }
    }

    void evscript_c::collisionCallback(collision2d_t& data){
        for(auto& script : scripts){
            script.collisionCallback(data);
        }
    }

    scriptHandler_t& evscript_c::addScript(const std::filesystem::path& path){
        for(auto& script : scripts){
            if(script.scriptpath == path) return script;
        }
        scripts.push_back(scriptHandler_t(entity, path));
        return scripts.back();
    }

    scriptHandler_t* evscript_c::tryGetScriptHandler(std::string name){
        for(auto& script : scripts){
            if(!name.compare(script.getScriptName())){
                EVLog_Msg("Found the script");
                return &script;
            }
        }
        return nullptr;
    }
}
}
