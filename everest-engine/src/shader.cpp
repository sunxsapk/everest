#include "renderer/shader.h"

namespace Everest {
    Shader::Shader(const char* vert_glsl, const char* frag_glsl){
        u32 vid = compileShader(vert_glsl, GL_VERTEX_SHADER);
        u32 fid = compileShader(vert_glsl, GL_FRAGMENT_SHADER);

        _id = linkShaders(vid, fid);

        glDeleteShader(vid);
        glDeleteShader(fid);
    }

    Shader::~Shader(){
        glDeleteProgram(_id);
    }

    void Shader::use(){
        glUseProgram(_id);
    }

    u32 Shader::compileShader(const char* glsl, GLenum type){
        ASSERT(glsl != NULL);
        ASSERT(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER);

        u32 shader_id = glCreateShader(type);
        glShaderSource(shader_id, 1, &glsl, NULL);
        glCompileShader(shader_id);

        i32 _success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &_success);
        if(!_success){
            char infoLog[512];
            glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
            EVLog_Err("%s Shader Compilation Failed:\n %s",
                    type == GL_VERTEX_SHADER? "Vertex" : "Fragment",
                    infoLog);
        }

        return shader_id;
    }

    u32 Shader::linkShaders(u32 vert, u32 frag){
        u32 prog_id = glCreateProgram();
        glAttachShader(prog_id, vert);
        glAttachShader(prog_id, frag);
        glLinkProgram(prog_id);

        i32 _success;
        glGetProgramiv(prog_id, GL_LINK_STATUS, &_success);
        if(!_success){
            char infoLog[512];
            glGetShaderInfoLog(prog_id, 512, NULL, infoLog);
            EVLog_Err("Shaders Linking Failed:\n %s", infoLog);
        }

        return prog_id;
    }
}
