#include "renderer/sprite.h"

namespace Everest {
    SpriteSheet::SpriteSheet(const char* filepath, uvec2 unitSize)
    :_texture(createRef<Texture>(filepath)), _unitSize(unitSize){ }

    SpriteSheet::SpriteSheet(ref<Texture>& sourceTexture, uvec2 unitSize)
    :_texture(sourceTexture), _unitSize(unitSize){
    }

    spriteRenderer_c SpriteSheet::getSprite(uvec2 start, uvec2 size){
        spriteRenderer_c sp;
        sp.texture = _texture;
        size += start;
        sp.startUV = (vec2)(start * _unitSize) / (vec2)_texture->getSize();
        sp.sizeUV  = (vec2)(size  * _unitSize) / (vec2)_texture->getSize();
        return sp;
    }
}
