/*
 * ========= Texture ===========
 * Author: Sunil Sapkota
 * Description: Textures are the image based assets. They can be passed to shader
 * to be sampled and used to fit the appropriate use case.
 */

#pragma once
#include "pch.h"

namespace Everest {
    enum TextureFormat{
        RED = GL_RED,
        RGB = GL_RGB,
        RGBA = GL_RGBA
    };

    enum TextureWrapMode {
        REPEAT = GL_REPEAT,
        CLAMP = GL_CLAMP_TO_EDGE,
    };

    enum TextureFilter {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR,
    };

    class Texture {
        public:
            Texture(const char* filepath,
                    TextureWrapMode wrap = CLAMP,
                    TextureFilter filter = NEAREST);
            Texture(ivec2 size, TextureFormat format = RGBA,
                    TextureWrapMode wrap = CLAMP,
                    TextureFilter filter = NEAREST);
            ~Texture();

            inline u32 getID(){return _id;}

            inline void bind(u32 slot = 0){
                EV_profile_function();
                
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_2D, _id);
            }

            inline u32 getWidth(){return _size.x;}
            inline u32 getHeight(){return _size.y;}
            inline uvec2 getSize(){return _size;}

            void setWrapMode(TextureWrapMode mode);
            void setFilter(TextureFilter filter);

            void setData(void* data ,u32 size);

            bool operator==(Texture& other){
                return _id ==  other.getID();
            }

        private:
            u32 _id;
            std::string _path;
            uvec2 _size;
            TextureFormat _format;
            TextureWrapMode _wrap;
            TextureFilter _filter;
    };
}
