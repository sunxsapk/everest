#include "renderer/buffer.h"

namespace Everest {

        GLenum BufferElement::getPrimitive() const{
            switch(type){

                case T_byte:
                case T_bvec2:
                case T_bvec3:
                case T_bvec4:
                case T_bool:    return GL_BYTE;

                case T_ubyte:   return GL_UNSIGNED_BYTE;

                case T_int:
                case T_ivec2:
                case T_ivec3:
                case T_ivec4:   return GL_INT;

                case T_uint:
                case T_uvec2:
                case T_uvec3:
                case T_uvec4:   return GL_UNSIGNED_INT;

                case T_float:
                case T_vec2:
                case T_vec3:
                case T_vec4:
                case T_mat3x3:
                case T_mat4x4:  return GL_FLOAT;

                case T_double:
                case T_dvec2:
                case T_dvec3:
                case T_dvec4:   return GL_DOUBLE;

                default: ASSERT(false, "Invalid type");
            }
            return GL_NONE;
        }

        u32 BufferElement::getCount() const{
            switch(type){
                case T_int:
                case T_uint:
                case T_float:
                case T_double:
                case T_bool:
                case T_byte:
                case T_ubyte: return 1;

                case T_bvec2:
                case T_ivec2:
                case T_uvec2:
                case T_vec2:
                case T_dvec2:   return 2;

                case T_bvec3:
                case T_ivec3:
                case T_uvec3:
                case T_vec3:
                case T_dvec3: return 3;

                case T_bvec4:
                case T_ivec4:
                case T_uvec4:
                case T_vec4:
                case T_dvec4: return 4;

                case T_mat3x3:  return 3*3;
                case T_mat4x4:  return 4*4;

                default: ASSERT(false, "Invalid type");
            }
            return 1;
        }

        u32 BufferElement::getSize() const {
            GLenum _type = getPrimitive();
            u32 _count = getCount();

            switch(_type){
                case GL_BYTE:
                    return _count * sizeof(GLbyte);
                case GL_UNSIGNED_BYTE:
                    return _count * sizeof(GLubyte);
                case GL_INT:
                    return _count * sizeof(GLint);
                case GL_UNSIGNED_INT:
                    return _count * sizeof(GLuint);
                case GL_FLOAT:
                    return _count * sizeof(GLfloat);
                case GL_DOUBLE:
                    return _count * sizeof(GLdouble);
            }
            return 0;
        }

    BufferLayout::BufferLayout(const layout_initlist_t& elements)
        : _elements(elements){
        _size = 0;
        for(auto& item : _elements){
            item.offset = _size;
            _size += item.getSize();
        }
    }

    Buffer::Buffer(bufd_t *_data, size_t _size, GLenum target)
        :_target(target){
        EV_profile_function();

        glGenBuffers(1, &_id);
        glBindBuffer(_target, _id);
        glBufferData(_target, _size, _data, _data?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);
        glBindBuffer(_target, 0);
    }

    Buffer::~Buffer(){
        EV_profile_function();


        glDeleteBuffers(1, &_id);
    }

    void Buffer::bind(){
        EV_profile_function();


        glBindBuffer(_target, _id);
    }

    void Buffer::unbind(){
        glBindBuffer(_target, 0);
    }

    VertexBuffer::VertexBuffer(bufd_t *_data, size_t _size)
        :Buffer(_data, _size, GL_ARRAY_BUFFER){ 
        EV_profile_function();
    }

    VertexBuffer::VertexBuffer(size_t _size)
        :Buffer(NULL, _size, GL_ARRAY_BUFFER){
    }

    void VertexBuffer::setData(const void* data, u32 size){
        glBindBuffer(_target, _id);
        glBufferData(_target, size, data, GL_DYNAMIC_DRAW);
        glBindBuffer(_target, 0);
    }

    IndexBuffer::IndexBuffer(u32 *_data, size_t _size)
        :Buffer(_data, _size, GL_ELEMENT_ARRAY_BUFFER){
        EV_profile_function();

        ASSERT(_data != NULL, "Can't pass null-pointer to index buffer");
        _count = _size / sizeof(_data[0]);
    }
}
