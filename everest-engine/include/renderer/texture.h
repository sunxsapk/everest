/*
 * ========= Texture ===========
 * Author: Sunil Sapkota
 * Description: Textures are the image based assets. They can be passed to shader
 * to be sampled and used to fit the appropriate use case.
 */

#pragma once

namespace Everest {
    class Texture {
        public:
            Texture(const std::string filepath);
            ~Texture();

            inline void bind(u32 slot = 0){
                glActiveTexture(GL_TEXTURE0+slot);
                glBindTexture(GL_TEXTURE_2D, _id);
            }

            inline u32 getWidth(){return _size.x;}
            inline u32 getHeight(){return _size.y;}
            inline uvec2 getSize(){return _size;}

        private:
            u32 _id;
            std::string _path;
            uvec2 _size;
    };
}
