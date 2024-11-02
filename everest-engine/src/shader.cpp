#include "renderer/shader.h"
#include <glm/gtc/type_ptr.hpp>

namespace Everest {
    Shader::Shader(const char* vert_glsl, const char* frag_glsl){
        u32 vid, fid;

        i32 vertex_shader_compiled = compileShader(vert_glsl, GL_VERTEX_SHADER, &vid);
        if(!vertex_shader_compiled){
            getInfoLog(vid, GL_VERTEX_SHADER);
            glDeleteShader(vid);
            ASSERT(vertex_shader_compiled);
        }

        i32 fragment_shader_compiled = compileShader(frag_glsl, GL_FRAGMENT_SHADER, &fid);
        if(!fragment_shader_compiled){
            getInfoLog(fid, GL_FRAGMENT_SHADER);
            glDeleteShader(vid);
            glDeleteShader(fid);
            ASSERT(fragment_shader_compiled);
        }

        linkShaders(vid, fid);
    }

    Shader::~Shader(){
        glDeleteProgram(_programID);
    }

    void Shader::bind(){
        glUseProgram(_programID);
    }

    void Shader::unbind(){
        glUseProgram(0);
    }

    void Shader::setUniform_Mat4(const std::string name, mat4 value){
        GLint loc = glGetUniformLocation(_programID, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(value));
    }

    i32 Shader::compileShader(const char* glsl, GLenum type, u32 *id){
        ASSERT(glsl != NULL);
        ASSERT(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER);

        *id = glCreateShader(type);
        glShaderSource(*id, 1, &glsl, NULL);
        glCompileShader(*id);

        i32 _shader_compiled;
        glGetShaderiv(*id, GL_COMPILE_STATUS, &_shader_compiled);

        return _shader_compiled;
    }

    void Shader::getInfoLog(u32 id, GLenum type){
        char infoLog[512];
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        EVLog_Err("%s Shader Compilation Failed:\n %s",
                type == GL_VERTEX_SHADER? "Vertex" : "Fragment",
                infoLog);
    }

    void Shader::linkShaders(u32 vert, u32 frag){
        _programID = glCreateProgram();
        glAttachShader(_programID, vert);
        glAttachShader(_programID, frag);
        glLinkProgram(_programID);

        i32 _shaders_linked;
        glGetProgramiv(_programID, GL_LINK_STATUS, &_shaders_linked);
        if(!_shaders_linked){
            char infoLog[512];
            glGetShaderInfoLog(_programID, 512, NULL, infoLog);
            EVLog_Err("Shaders Linking Failed:\n %s", infoLog);

            glDeleteShader(vert);
            glDeleteShader(frag);
            glDeleteProgram(_programID);
            ASSERT(_shaders_linked);
        }

        glDetachShader(_programID, vert);
        glDetachShader(_programID, frag);
        glDeleteShader(vert);
        glDeleteShader(frag);
    }
}
