#include "renderer/renderer2d.h"
#include "renderer/renderAPI.h"

namespace Everest {
    RendererStorage *Renderer2D::_data = new RendererStorage;

    void Renderer2D::init(){
        f32 verts[] = {
            -0.5f, -0.5f,  0.5f, 0.f, 0.f,
            0.5f, -0.5f,  0.5f, 1.f, 0.f,
            -0.5f,  0.5f,  0.5f, 0.f, 1.f,
            0.5f,  0.5f,  0.5f, 1.f, 1.f,
        };
        u32 inds[] = { 0, 1, 2, 1, 3, 2 };


        BufferLayout layout {
            {ShaderDataType::T_vec3, "aPos"},
                {ShaderDataType::T_vec2, "aUV"},
        };

        _data->vertArray = createRef<VAO>();
        ref<VertexBuffer> vb = createRef<VertexBuffer>(verts, sizeof(verts));
        vb->setLayout(layout);
        ref<IndexBuffer> ib = createRef<IndexBuffer>(inds, sizeof(inds)); 
        _data->vertArray->addVertexBuffer(vb);
        _data->vertArray->addIndexBuffer(ib);

        _data->textureShader = createRef<Shader>("./assets/shaders/textureshader.glsl");
        _data->whiteTexture = createRef<Texture>(uvec2(1));
        u32 whiteText_data = 0xffffffff;
        _data->whiteTexture->setData(&whiteText_data, 4);
    }

    void Renderer2D::quit(){
        delete _data;
    }

    void Renderer2D::beginScene(Camera& camera){
        _data->textureShader->bind();
        _data->textureShader->setUniform_Mat4("u_viewProjMat", camera.getVPmatrix());
        _data->textureShader->setUniform_i32("u_texture", 0);
    }

    void Renderer2D::endScene(){
    }

    void Renderer2D::drawQuad(vec3 position, vec2 scale, f32 rotation, vec4 color, ref<Texture> texture){
        static const vec3 _z_axis = vec3(0.f, 0.f, 1.f);
        mat4 transform(1.f);
        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, glm::radians(rotation), _z_axis);
        transform = glm::scale(transform, vec3(scale.x, scale.y, 1.f));

        _data->textureShader->setUniform_Mat4("u_transform", transform);
        _data->textureShader->setUniform_vec4("u_color", color);

        if(texture != NULL) texture->bind();
        else _data->whiteTexture->bind();

        RenderAPI::drawIndexed(_data->vertArray);
    }

    void Renderer2D::drawQuad(vec2 position, vec2 scale, f32 rotation, vec4 color, ref<Texture> texture){
        drawQuad(vec3(position.x, position.y, 0.f), scale, rotation, color, texture);
    }
}
