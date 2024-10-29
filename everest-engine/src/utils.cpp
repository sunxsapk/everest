#include "math/utils.h"

namespace Everest {
    size_t glEnum2Size(GLenum _enum){
        switch(_enum){
						case GL_BYTE:
                return sizeof(GLbyte);
						case GL_UNSIGNED_BYTE:
                return sizeof(GLubyte);
						case GL_SHORT:
                return sizeof(GLshort);
						case GL_UNSIGNED_SHORT:
                return sizeof(GLushort);
						case GL_INT:
                return sizeof(GLint);
						case GL_UNSIGNED_INT:
                return sizeof(GLuint);
						case GL_FIXED:
                return sizeof(GLfixed);
						case GL_HALF_FLOAT:
                return sizeof(GLhalf);
						case GL_FLOAT:
                return sizeof(GLfloat);
						case GL_DOUBLE:
                return sizeof(GLdouble);
        }
        return 0;
    }
}
