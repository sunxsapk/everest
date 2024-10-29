/*
 * ========= shader ===========
 * Author: Sunil Sapkota
 * Description: shader class provides all the functionality to set, compile and
 * link shader(glsl) code.
 */

#pragma once

namespace Everest {
    class Shader {
        public:
            Shader(const char* vert_glsl, const char* frag_glsl);
            ~Shader();

            void use();

        private:
            u32 _id;
            u32 compileShader(const char* glsl, GLenum type);
            u32 linkShaders(u32 vert, u32 frag);
    };
}
