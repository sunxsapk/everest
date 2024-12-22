#include "renderer/texture.h"

namespace Everest {
    Texture::Texture(ivec2 size, TextureFormat format, TextureWrapMode wrap, TextureFilter filter)
    :_size(size), _format(format){
        EV_profile_function();

        ASSERT(_format != 0, "Unsupported format detected");

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        setWrapMode(wrap);
        setFilter(filter);
    }

    void Texture::setData(void* data ,u32 size){
        EV_profile_function();

#if ASSERT_ON
        u32 sz = _format==RGBA?4:(_format==RGB?3:1);
        sz *= _size.x * _size.y;
        ASSERT(size == sz, "Invalid data size");
#endif
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexImage2D(GL_TEXTURE_2D, 0, _format, _size.x, _size.y, 0,
                _format, GL_UNSIGNED_BYTE, data);
        
        EVLog_Msg("Texture : %d", _id);
    }

    Texture::Texture(const char* filepath, TextureWrapMode wrap, TextureFilter filter)
        :_path(filepath){
        EV_profile_function();


        i32 width, height, channels;

        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data;
        {
            EV_profile_scope("Texture image load");
            data = stbi_load(_path.c_str(), &width, &height, &channels, 0);
        }
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

        setWrapMode(wrap);
        setFilter(filter);

        glTexImage2D(GL_TEXTURE_2D, 0, _format, _size.x, _size.y, 0,
                _format, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    void Texture::setWrapMode(TextureWrapMode mode){
        _wrap = mode;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap);
    }

    void Texture::setFilter(TextureFilter filter){
        _filter = filter;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter);
    }

    Texture::~Texture(){
        EV_profile_function();

        glDeleteTextures(1, &_id);
    }
}
