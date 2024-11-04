/*
 * ========= shader ===========
 * Author: Sunil Sapkota
 * Description: shader class provides all the functionality to set, compile and
 * link shader(glsl) code.
 */

#pragma once

#include "math/types.h"
#include <glm/gtc/type_ptr.hpp>

namespace Everest {

    class Shader {
        public:
            Shader(const char* vert_glsl, const char* frag_glsl);
            ~Shader();

            void bind();
            void unbind();

            using name_t = const std::string;

            inline void setUniform_i32(name_t name, i32 value){
                glUniform1i(_getloc(name), value);
            }
            inline void setUniform_f32(name_t name, f32 value){
                glUniform1f(_getloc(name), value);
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
                ASSERT(loc != -1);
                _uniformCache[name] = loc;
                return loc;
            }

        private:
            u32 _programID;
            i32 compileShader(const char* glsl, GLenum type, u32 *id);
            void linkShaders(u32 vert, u32 frag);
            void getInfoLog(u32 id, GLenum type);
    };
}
