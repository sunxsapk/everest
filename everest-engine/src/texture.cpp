#include "renderer/texture.h"

namespace Everest {
    Texture::Texture(const std::string filepath)
        :_path(filepath){

        i32 width, height, channels;

        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(_path.c_str(), &width, &height, &channels, 0);
        ASSERT(data != NULL);

        _size = {width, height};
        EVLog_Msg("%s : %d, %d, %d", filepath.c_str(), width, height, channels);

        GLenum _srcformat = 0, _trgformat;
        if(channels == 4){
            _srcformat = GL_RGBA;
            _trgformat = GL_RGBA;
        } else if (channels == 3){
            _srcformat = GL_RGB;
            _trgformat = GL_RGB;
        } else if (channels == 1){
            _srcformat = GL_RED;
            _trgformat = GL_RED;
        }
        ASSERT(_srcformat != 0);

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, _trgformat, _size.x, _size.y, 0,
                _srcformat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    Texture::~Texture(){
        glDeleteTextures(1, &_id);
    }
}
