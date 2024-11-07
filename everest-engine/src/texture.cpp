#include "renderer/texture.h"

namespace Everest {
    Texture::Texture(ivec2 size, TextureFormat format)
    :_size(size), _format(format){
        ASSERT(_format != 0, "Unsupported format detected");

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void Texture::setData(void* data
#if ASSERT_ON
            ,u32 size
#endif
        ){
#if ASSERT_ON
        u32 sz = _format==RGBA?4:(_format==RGB?3:1);
        sz *= _size.x * _size.y;
        ASSERT(size == sz, "Invalid data size");
#endif
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexImage2D(GL_TEXTURE_2D, 0, _format, _size.x, _size.y, 0,
                _format, GL_UNSIGNED_BYTE, data);
    }

    Texture::Texture(const std::string filepath)
        :_path(filepath){

        i32 width, height, channels;

        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(_path.c_str(), &width, &height, &channels, 0);
        ASSERT(data != NULL, "Failed to load texture");

        _size = {width, height};

        if(channels == 4){
            _format = RGBA;
        } else if (channels == 3){
            _format = RGB;
        } else if (channels == 1){
            _format = RED;
        }
        ASSERT(_format != 0, "Unsupported format detected");

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, _format, _size.x, _size.y, 0,
                _format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    Texture::~Texture(){
        glDeleteTextures(1, &_id);
    }
}
