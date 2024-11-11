/*
 * ========= shader ===========
 * Author: Sunil Sapkota
 * Description: shader class provides all the functionality to set, compile and
 * link shader(glsl) code.
 */

#pragma once
#include "pch.h"

#include "math/types.h"
#include <glm/gtc/type_ptr.hpp>

namespace Everest {

    class Shader {
        public:
            Shader(const std::string name, const std::string vert_glsl, const std::string frag_glsl);
            Shader(const std::string filepath);
            ~Shader();

            void bind();
            void unbind();
            inline const std::string& getName(){return _name;}

            using name_t = const std::string;

            inline void setUniform_i32(name_t name, i32 value){
                glUniform1i(_getloc(name), value);
            }
            inline void setUniform_f32(name_t name, f32 value){
                glUniform1f(_getloc(name), value);
            }
            inline void setUniform_iarr(name_t name, i32* value, u32 count){
                glUniform1iv(_getloc(name), count, value);
            }

            inline void setUniform_ivec2(name_t name, ivec2 value){
                glUniform2iv(_getloc(name), 1, glm::value_ptr(value));
            }
            inline void setUniform_vec2(name_t name, vec2 value){
                glUniform2fv(_getloc(name), 1, glm::value_ptr(value));
            }
            inline void setUniform_dvec2(name_t name, dvec2 value){
                glUniform2dv(_getloc(name), 1, glm::value_ptr(value));
            }

            inline void setUniform_ivec3(name_t name, ivec3 value){
                glUniform3iv(_getloc(name), 1, glm::value_ptr(value));
            }
            inline void setUniform_vec3(name_t name, vec3 value){
                glUniform3fv(_getloc(name), 1, glm::value_ptr(value));
            }
            inline void setUniform_dvec3(name_t name, dvec3 value){
                glUniform3dv(_getloc(name), 1, glm::value_ptr(value));
            }

            inline void setUniform_ivec4(name_t name, ivec4 value){
                glUniform4iv(_getloc(name), 1, glm::value_ptr(value));
            }
            inline void setUniform_vec4(name_t name, vec4 value){
                glUniform4fv(_getloc(name), 1, glm::value_ptr(value));
            }
            inline void setUniform_dvec4(name_t name, dvec4 value){
                glUniform4dv(_getloc(name), 1, glm::value_ptr(value));
            }

            inline void setUniform_Mat4(name_t name, mat4 value){
                glUniformMatrix4fv(_getloc(name), 1, 0, glm::value_ptr(value));
            }
            inline void setUniform_Mat3(name_t name, mat3 value){
                glUniformMatrix3fv(_getloc(name), 1, 0, glm::value_ptr(value));
            }

        private:
            std::unordered_map<std::string, int> _uniformCache;

            i32 _getloc(name_t name){
                const auto& _it = _uniformCache.find(name);
                if(_it != _uniformCache.end()) return _it->second;

                GLint loc = glGetUniformLocation(_programID, name.c_str());
                ASSERT(loc != -1, "Uniform variable '%s' not found", name.c_str());
                _uniformCache[name] = loc;
                return loc;
            }

        private:
            u32 _programID;
            std::string _name;

        private:
            void _compile(const char* vert, const char* frag);
            i32 _compSh(const char* glsl, GLenum type, u32 *id);
            std::string _readSh(const std::string& filepath);
            void _linkSh(u32 vert, u32 frag);
            void _getIL(u32 id, GLenum type);
    };

    class ShaderLibrary {
        public:
            void Add(const std::string name, ref<Shader>& shader);
            void Add(ref<Shader>& shader);

            ref<Shader> load(const std::string filepath);
            ref<Shader> load(const std::string filepath, const std::string name);

            bool exists(const std::string name);
            ref<Shader> get(const std::string name);

        private:
            std::unordered_map<std::string, ref<Shader>> _shaders;
    };
}
