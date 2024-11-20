/*
 * ============= Components ===============
 * Author: Sunil Sapkota
 * Description : Contains all of the components made available by the engine.
 */
#pragma once


namespace Everest {
    struct transform_c {
        mat4 transform{1.f};

        operator mat4&(){return transform;}
        operator const mat4&()const{return transform;}
    };

    struct tag_c {
        const char* tag;
    };
    
    struct spriteRenderer_c{
        vec4 color{1.f}; 
    };
}
