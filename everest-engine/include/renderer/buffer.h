/*
 * ========== buffer ===========
 * Author: Sunil Sapkota
 * Description: buffer class and its childs provide the wrapper around opengl's
 * buffers. some of the buffers implementation are vertex buffers, element
 * buffers, etc.
 */

#pragma once

namespace Everest {

    enum ShaderDataType {
        NONE = 0,

        T_bool,
        T_byte,
        T_ubyte,

        T_int,
        T_uint,
        T_float,
        T_double,

        T_bvec2,
        T_ivec2,
        T_uvec2,
        T_vec2,
        T_dvec2,

        T_bvec3,
        T_ivec3,
        T_uvec3,
        T_vec3,
        T_dvec3,

        T_bvec4,
        T_ivec4,
        T_uvec4,
        T_vec4,
        T_dvec4,

        T_mat3x3,
        T_mat4x4,
    };

    struct BufferElement {
        u32 offset;
        ShaderDataType type;
        std::string name;

        BufferElement(ShaderDataType _type, std::string _name)
            : type(_type), name(_name) { }

        GLenum getPrimitive() const;
        u32 getCount() const;
        u32 getSize() const;
    };

    class BufferLayout {
        using layout_t = std::vector<BufferElement>;
        using layout_initlist_t = std::initializer_list<BufferElement>;
        public:
            BufferLayout(){}
            BufferLayout(const layout_initlist_t& elements);

            inline u32 getSize() const {return _size;}

            layout_t::iterator begin(){return _elements.begin();}
            layout_t::iterator end(){return _elements.end();}

            layout_t::const_iterator begin() const {return _elements.begin();}
            layout_t::const_iterator end() const {return _elements.end();}
        private:
            u32 _size = 0;
            layout_t _elements;
    };

    using bufd_t = const void;

    class Buffer {
        public:
            ~Buffer();

            /*binds the buffer*/
            virtual void bind();
            /*unbinds the buffer*/
            virtual void unbind();
        protected:
            u32 _id;
            GLenum _target;
            Buffer(bufd_t *_data, size_t _size, GLenum target);
    };

    class VertexBuffer : public Buffer {
        public:
            /* creates, binds and fill the vertex-buffer with data. Buffer is
             * not unbinded when it finishes creating. You have to unbind it
             * later. Most probably only after unbinding the vertex array.*/
            VertexBuffer(bufd_t *_data, size_t _size);

            inline void setLayout(const BufferLayout layout){_layout = layout;}
            inline const BufferLayout& getLayout(){return _layout;}
        private:
            BufferLayout _layout;
    };

    class IndexBuffer : public Buffer {
        public:
            /* creates, binds and fill the index/element-buffer with data.
             * Buffer is not unbinded when it finishes creating. You have to
             * unbind it later. Most probably only after unbinding the vertex
             * array.*/
            IndexBuffer(u32 *_data, size_t _size);
            /*returns the number of indices in the buffer*/
            inline u32 getCount(){return _count;}
        private:
            u32 _count;
    };
}
