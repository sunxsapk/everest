/*
 * ========= Sprites =========
 * Author: Sunil Sapkota
 * Description: Sprites are the esseential components for 2d games. They provide
 * a wrapper around textures and support feature for tile-atlases.
 */

#pragma once
#include "texture.h"


namespace Everest {
    struct Sprite {
        ref<Texture> texture = nullptr;
        vec2 startUV = vec2(0.f);
        vec2 sizeUV  = vec2(1.f);
    };

    class SpriteSheet {
        public:
            SpriteSheet():_unitSize(0){}
            /* creates a spritesheet
             * @param sourcePath : path to sprite source
             * @param unitSize : a 2-component vector representing texture's unit
             * size in pixels for reference. Unitsize will be used to convert
             * coordinates in atlas-unit to pixels */
            SpriteSheet(const char* sourcePath, uvec2 unitSize);
            /* creates a spritesheet from given source texture, with sprite unit-
             * size of "unitSize" */
            SpriteSheet(ref<Texture> sourceTexture, uvec2 unitSize);

            /* takes the coordinate of bottom left corner and size of sprite in 
             * sprite atlas unit, and returns the uv coordinate in the form :
             * (x, y, w, h)*/
            Sprite getSprite(uvec2 start, uvec2 size);

            inline uvec2 getUnitSize(){return _unitSize;}
            inline uvec2 getSpriteSize(){return _texture?
                _texture->getSize() : uvec2(0);}
            inline ref<Texture> getTexture(){return _texture;}

        private:
            ref<Texture> _texture;
            uvec2 _unitSize;
    };
}
