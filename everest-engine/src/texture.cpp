#include "renderer/texture.h"

namespace Everest {
    Texture::Texture(const std::string filepath)
        :_path(filepath){

        i32 width, height, channels;

        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(_path.c_str(), &width, &height, &channels, 0);
        ASSERT(data != NULL);

        _size = {width, height};

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _size.x, _size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    Texture::~Texture(){
        glDeleteTextures(1, &_id);
    }
}
