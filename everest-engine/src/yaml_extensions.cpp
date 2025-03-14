#include "scene/yaml_extensions.h"


namespace Everest {
    YAML::Emitter& operator<<(YAML::Emitter& out, const vec2& v){
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const vec3& v){
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const vec4& v){
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const transform_c& tfr){
        using namespace YAML;
        out << Key << "transform_c";
        out << BeginMap;
        
        out << Key << "position" << Value << tfr.position;
        out << Key << "rotation" << Value << tfr.rotation;
        out << Key << "scale" << Value << tfr.scale;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const tag_c& tag){
        using namespace YAML;
        out << Key << "tag_c";
        out << BeginMap;

        out << Key << "tag" << Value << tag.tag;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const camera_c& camera){
            using namespace YAML;
        out << Key << "camera_c";
        out << BeginMap;

        out << Key << "active" << Value << camera.active;
        out << Key << "fixedAspect" << Value << camera.fixedAspect;
        out << Key << "isPrimary" << Value << camera.isPrimary;
        out << Key << "is2d" << Value << camera.is2d();

        out << Key << "u_size_fov" << Value << camera.get_lenssize();
        out << Key << "aspect" << Value << camera.get_aspect();
        out << Key << "near" << Value << camera.get_near();
        out << Key << "far" << Value << camera.get_far();

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const spriteRenderer_c& spriteRenderer){
        using namespace YAML;
        out << Key << "spriteRenderer_c";
        out << BeginMap;

        out << Key << "active" << Value << spriteRenderer.active;
        out << Key << "color" << Value << spriteRenderer.color;
        out << Key << "startUV" << Value << spriteRenderer.startUV;
        out << Key << "sizeUV" << Value << spriteRenderer.sizeUV;

        const ref<Texture>& tx = spriteRenderer.texture;
        if(tx != nullptr) out << Key << "texturePath" << Value << tx->getPath();

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const circleRenderer_c& circleRenderer){
        using namespace YAML;
        out << Key << "circleRenderer_c";
        out << BeginMap;

        out << Key << "active" << Value << circleRenderer.active;
        out << Key << "color" << Value << circleRenderer.color;
        out << Key << "startUV" << Value << circleRenderer.startUV;
        out << Key << "sizeUV" << Value << circleRenderer.sizeUV;
        out << Key << "thickness" << Value << circleRenderer.thickness;
        out << Key << "fade" << Value << circleRenderer.fade;

        const ref<Texture>& tx = circleRenderer.texture;
        if(tx != nullptr) out << Key << "texturePath" << Value << tx->getPath();

        out << EndMap;
        return out;
    }


    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody2d_c& rb2d){
        using namespace YAML;
        out << Key << "rigidbody2d_c";
        out << BeginMap;

        out << Key << "active" << Value << rb2d.active;
        out << Key << "inverseMass" << Value << rb2d.inverseMass;
        out << Key << "drag" << Value << rb2d.drag;
        out << Key << "velocity" << Value << rb2d.velocity;
        out << Key << "angularVelocity" << Value << rb2d.angularVelocity;
        out << Key << "definition" << Value << rb2d.definition;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, springJoint2d_c& spr){
        using namespace YAML;
        out << Key << "springJoint2d_c";
        out << BeginMap;

        out << Key << "active" << Value << spr.active;
        out << Key << "anchor" << Value << (spr.anchor.isValid() ? (u64) spr.anchor.get<Everest::id_c>().id : 0);
        out << Key << "offset" << Value << spr.offset;
        out << Key << "springConstant" << Value << spr.springConstant;
        out << Key << "damping" << Value << spr.damping;
        out << Key << "restLength" << Value << spr.restLength;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const boxCollider2d_c& boxCollider2d){
        using namespace YAML;
        out << Key << "boxCollider2d_c";
        out << BeginMap;

        out << Key << "active" << Value << boxCollider2d.active;
        out << Key << "offset" << Value << boxCollider2d.box.offset;
        out << Key << "halfExtents" << Value << boxCollider2d.box.halfExtents;
        out << Key << "restitution" << Value << boxCollider2d.restitution;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const circleCollider2d_c& circleCollider2d){
        using namespace YAML;
        out << Key << "circleCollider2d_c";
        out << BeginMap;

        out << Key << "active" << Value << circleCollider2d.active;
        out << Key << "offset" << Value << circleCollider2d.circle.offset;
        out << Key << "radius" << Value << circleCollider2d.circle.radius;
        out << Key << "restitution" << Value << circleCollider2d.restitution;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const EvScript& script){ // TODO: active
        using namespace YAML;
        using namespace Scripting;
        if(!script.scripts.size()) return out;

        out << Key << "evscript_c";
        out << BeginSeq;
        for(auto& sh : script.scripts){
            out << BeginMap;
            out << Key << "path" << Value << sh.scriptpath.c_str();

            luastate_t& st = *sh.state;
            sol::table ser;
            if(sh.getSerializedFields(ser)) {
                out << Key << "SERIALIZE";
                out << BeginSeq;
                for(auto& [key, type] : ser){
                    if(!key.valid() || !type.valid()) continue;
                    auto val = st[key.as<std::string>()];
                    if(!val.valid()) continue;

                    out << BeginMap;
                    out << Key << "name" << Value << key.as<std::string>();
                    out << Key << "type" << Value << (u32)type.as<Types>();
                    out << Key << "value" << Value;

                    switch(type.as<Types>()){
                        case Types::Int:
                            out << (i32)val;
                            break;
                        case Types::Bool:
                            out << (bool)val;
                            break;
                        case Types::Float:
                            out << (f64)val;
                            break;
                        case Types::String: {
                            std::string vstr = val;
                            out << vstr;
                            break;
                        }
                        case Types::Vec2:
                            out << (vec2&)val;
                            break;
                        case Types::Vec3:
                            out << (vec3&)val;
                            break;
                        case Types::Vec4:
                        case Types::Color:
                            out << (vec4&)val;
                            break;
                        case Types::Entity:
                            out << (u64)((Entity&)val).get<id_c>().id;
                            break;
                    }
                    out << EndMap;
                }
                out << EndSeq;
            }

            out << EndMap;
        }
        out << EndSeq;
        return out;
    }

/*#ifndef __NO_3D__
    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody_c& rb){
        using namespace YAML;
        out << Key << "rigidbody_c";
        out << BeginMap;

        out << Key << "active" << Value << rb.active;
        out << Key << "inverseMass" << Value << rb.inverseMass;
        out << Key << "drag" << Value << rb.drag;
        out << Key << "velocity" << Value << rb.velocity;
        out << Key << "angularVelocity" << Value << rb.angularVelocity;
        out << Key << "useGravity" << Value << rb.useGravity;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const springJoint_c& spr){
        using namespace YAML;
        out << Key << "springJoint_c";
        out << BeginMap;

        out << Key << "active" << Value << spr.active;
        out << Key << "anchor" << Value << spr.anchor;
        out << Key << "offset" << Value << spr.offset;
        out << Key << "springConstant" << Value << spr.springConstant;
        out << Key << "damping" << Value << spr.damping;
        out << Key << "restLength" << Value << spr.restLength;

        out << EndMap;
        return out;
    }
#endif*/
}
