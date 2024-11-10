/*
 * ========= Texture ===========
 * Author: Sunil Sapkota
 * Description: Textures are the image based assets. They can be passed to shader
 * to be sampled and used to fit the appropriate use case.
 */

#pragma once

namespace Everest {
    enum TextureFormat{
        RED = GL_RED,
        RGB = GL_RGB,
        RGBA = GL_RGBA
    };

    class Texture {
        public:
            Texture(const std::string filepath);
            Texture(ivec2 size, TextureFormat format = RGBA);
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

            inline void setRepeat(){
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }

            inline void setClamp(){
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            void setData(void* data
#if ASSERT_ON
                    ,u32 size
#endif
                    );

            bool operator==(Texture& other){
                return _id ==  other.getID();
            }

        private:
            u32 _id;
            std::string _path;
            uvec2 _size;
            TextureFormat _format;
    };
}
