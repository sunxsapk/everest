#include "renderer/shader.h"

namespace Everest {
    GLenum getType(std::string token){
        if(token == "vertex") return GL_VERTEX_SHADER;
        if(token == "fragment" || token == "pixel") return GL_FRAGMENT_SHADER;
        ASSERT(false, "Unknown shader type");
        return 0;
    }

    std::unordered_map<GLenum, std::string> getShaders(std::string src){
        EV_profile_function();

        std::unordered_map<GLenum, std::string> shaders;

        const char* type_token = "#type";
        size_t ttoken_l = strlen(type_token);

        size_t pos = src.find(type_token);
        while(pos != std::string::npos){
            size_t eol = src.find_first_of("\r\n", pos);
            ASSERT(eol != std::string::npos, "Shader syntax err: no shader \
                    source after #type token");
            size_t tkpos = pos + ttoken_l + 1;
            std::string tname = src.substr(tkpos, eol-tkpos);

            size_t shbeg = src.find_first_not_of("\r\n", eol);
            pos = src.find(type_token, shbeg);
            shaders[getType(tname)] = src.substr(shbeg, pos - 
                    (shbeg == std::string::npos?src.size()-1:shbeg));
        }

        return shaders;
    }

    Shader::Shader(const std::string name, const std::string vert_glsl, const std::string frag_glsl)
    : _name(name){
        EV_profile_function();

        _compile(vert_glsl.c_str(), frag_glsl.c_str());
    }

    Shader::Shader(const std::string filepath){
        EV_profile_function();

        std::string src = _readSh(filepath);
        std::unordered_map<GLenum, std::string> shaders = getShaders(src);
        _compile(shaders[GL_VERTEX_SHADER].c_str(), shaders[GL_FRAGMENT_SHADER].c_str());

        size_t _beg = filepath.find_last_of("/\\");
        _beg = _beg == std::string::npos?0:(_beg+1);
        size_t _end = filepath.find_last_of(".");
        _end = _end == std::string::npos?filepath.size() : _end;

        _name = filepath.substr(_beg, _end -_beg);
    }

    std::string Shader::_readSh(const std::string& filepath){
        EV_profile_function();

        std::ifstream f_in(filepath, std::ios::in | std::ios::binary);
        ASSERT(f_in.is_open(), "Invalid file: %s", filepath.c_str());

        std::string content;
        f_in.seekg(0, std::ios::end);
        content.resize(f_in.tellg());
        f_in.seekg(0, std::ios::beg);
        f_in.read(&content[0], content.size());
        f_in.close();
        return content;
    }
    void Shader::_compile(const char* vert_glsl, const char* frag_glsl){
        EV_profile_function();

        u32 vid, fid;

        i32 _success = _compSh(vert_glsl, GL_VERTEX_SHADER, &vid);
        if(!_success){
            _getIL(vid, GL_VERTEX_SHADER);
            glDeleteShader(vid);
            ASSERT(_success, "Failed to compile vertex shader");
        }

        _success = _compSh(frag_glsl, GL_FRAGMENT_SHADER, &fid);
        if(!_success){
            _getIL(fid, GL_FRAGMENT_SHADER);
            glDeleteShader(vid);
            glDeleteShader(fid);
            ASSERT(_success, "Failed to compile fragment shader");
        }

        _linkSh(vid, fid);
    }

    Shader::~Shader(){
        EV_profile_function();

        glDeleteProgram(_programID);
        EVLog_Msg("Shader deleted");
    }

    void Shader::bind(){
        EV_profile_function();

        glUseProgram(_programID);
    }

    void Shader::unbind(){
        EV_profile_function();

        glUseProgram(0);
    }

    i32 Shader::_compSh(const char* glsl, GLenum type, u32 *id){
        EV_profile_function();

        ASSERT(glsl != NULL, "Cannot pass null string for compilation");
        ASSERT(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER, 
                "Invalid shader type");

        *id = glCreateShader(type);
        glShaderSource(*id, 1, &glsl, NULL);
        glCompileShader(*id);

        i32 _shader_compiled;
        glGetShaderiv(*id, GL_COMPILE_STATUS, &_shader_compiled);

        return _shader_compiled;
    }

    void Shader::_getIL(u32 id, GLenum type){
        EV_profile_function();

        char infoLog[512];
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        EVLog_Err("%s Shader Compilation Failed:\n %s",
                type == GL_VERTEX_SHADER? "Vertex" : "Fragment",
                infoLog);
    }

    void Shader::_linkSh(u32 vert, u32 frag){
        EV_profile_function();

        _programID = glCreateProgram();
        glAttachShader(_programID, vert);
        glAttachShader(_programID, frag);
        glLinkProgram(_programID);

        i32 _shaders_linked;
        glGetProgramiv(_programID, GL_LINK_STATUS, &_shaders_linked);
        if(!_shaders_linked){
            char infoLog[512];
            glGetProgramInfoLog(_programID, 512, NULL, infoLog);
            EVLog_Err("Shaders Linking Failed:\n %s", infoLog);

            glDeleteShader(vert);
            glDeleteShader(frag);
            glDeleteProgram(_programID);
            ASSERT(_shaders_linked, "Failed to link shaders");
        }

        glDetachShader(_programID, vert);
        glDetachShader(_programID, frag);
        glDeleteShader(vert);
        glDeleteShader(frag);
    }


    void ShaderLibrary::Add(const std::string name, ref<Shader>& shader){
        EV_profile_function();

        ASSERT(_shaders.find(name) == _shaders.end(), "Shader already exists");
        _shaders[name] = shader;
    }

    void ShaderLibrary::Add(ref<Shader>& shader){
        EV_profile_function();

        Add(shader->getName(), shader);
    }

    ref<Shader> ShaderLibrary::load(const std::string filepath){
        EV_profile_function();

        ref<Shader> shader = createRef<Shader>(filepath);
        Add(shader);
        return shader;
    }

    ref<Shader> ShaderLibrary::load(const std::string filepath,
            const std::string name){
        EV_profile_function();

        ref<Shader> shader = createRef<Shader>(filepath);
        Add(name, shader);
        return shader;
    }

    bool ShaderLibrary::exists(const std::string name){
        EV_profile_function();

        return _shaders.find(name) != _shaders.end();
    }

    ref<Shader> ShaderLibrary::get(const std::string name){
        EV_profile_function();

        ASSERT(_shaders.find(name) != _shaders.end(), "Shader not found");
        return _shaders[name];
    }
}
