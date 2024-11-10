#include "renderer/renderer2d.h"
#include "renderer/renderAPI.h"

namespace Everest {
    Renderer2Ddata* Renderer2D::_data = new Renderer2Ddata;

    void Renderer2D::init(){
        EV_profile_function();

        _data->vertBase = new QuadVertex[_data->maxVertices];
        _data->vertPtr = _data->vertBase;

        ref<VertexBuffer> qvb = createRef<VertexBuffer>(_data->maxVertices * sizeof(QuadVertex));
        BufferLayout layout {
            {ShaderDataType::T_vec3, "position"},
            {ShaderDataType::T_vec4, "color"},
            {ShaderDataType::T_vec2, "uv"},
            {ShaderDataType::T_float, "textureIndex"},
            {ShaderDataType::T_float, "tilingFactor"},
        };
        qvb->setLayout(layout);
        _data->vertArray = createRef<VAO>();
        _data->vertArray->addVertexBuffer(qvb);

        u32 *qind = new u32[_data->maxIndices];
        for(u32 i=0, offset=0; i<_data->maxIndices; i+=6, offset+=4){
            qind[i + 0] = 0 + offset;
            qind[i + 1] = 1 + offset;
            qind[i + 2] = 2 + offset;
            qind[i + 3] = 2 + offset;
            qind[i + 4] = 3 + offset;
            qind[i + 5] = 0 + offset;
        }

        ref<IndexBuffer> qib = createRef<IndexBuffer>(qind, sizeof(u32) * _data->maxIndices);
        _data->vertArray->addIndexBuffer(qib);
        delete[] qind;

        _data->textureShader = createRef<Shader>("assets/shaders/batchedShader.glsl");
        i32 samplerArr[32];
        for(u32 i=0; i<_data->maxTexSlots; i++) samplerArr[i] = i;
        _data->textureShader->setUniform_iarr("u_textures", samplerArr, _data->maxTexSlots);

        _data->whiteTexture = createRef<Texture>(uvec2(1));
        u32 whiteText_data = 0xffffffff;
        _data->whiteTexture->setData(&whiteText_data, 4);
    }

    void Renderer2D::quit(){
        EV_profile_function();

        delete[] _data->vertBase;
        delete _data;
    }

    void Renderer2D::beginScene(Camera& camera){
        EV_profile_function();

        _data->textureShader->bind();
        _data->textureShader->setUniform_Mat4("u_viewProjMat", camera.getVPmatrix());

        _data->whiteTexture->bind();

        _data->indexCount = 0;
        _data->texCount = 1; //white texture
        _data->vertPtr = _data->vertBase;

        _data->textures[0] = _data->whiteTexture;
        _data->texCount = 1;
    }

    void Renderer2D::endScene(){
        EV_profile_function();

        flush();
    }

    void Renderer2D::flush(){
        u32 datasize = (u8*)_data->vertPtr - (u8*)_data->vertBase;
        _data->vertArray->getVertexBuffer()->setData(_data->vertBase, datasize);

        for(u32 i=0; i<_data->texCount; i++){
            _data->textures[i]->bind(i);
        }

        RenderAPI::drawIndexed(_data->vertArray, _data->indexCount);

        _data->indexCount = 0;
        _data->texCount = 1;
        _data->vertPtr = _data->vertBase;
    }

    void Renderer2D::drawQuad(vec3 position, vec2 scale, f32 rotation,
            vec4 color, ref<Texture> texture, f32 tilingFactor){
        EV_profile_function();

        f32 tind = 0;
        if(texture != NULL){
            for(u32 i=1; i<_data->texCount; i++){
                if(_data->textures[i]->getID() == texture->getID()){
                    tind = i;
                    break;
                }
            }
            if(tind == 0){
                EVLog_Msg("slot : %d, texture : %d", _data->texCount, texture->getID());
                _data->textures[_data->texCount++] = texture;
            }
        }

        _data->vertPtr->position = position;
        _data->vertPtr->color = color;
        _data->vertPtr->uv = {0.f, 0.f};
        _data->vertPtr->textureIndex = tind;
        _data->vertPtr->tilingFactor = tilingFactor;
        _data->vertPtr++;

        _data->vertPtr->position = position + vec3(1.f, 0.f, 0.f);
        _data->vertPtr->color = color;
        _data->vertPtr->uv = {1.f, 0.f};
        _data->vertPtr->textureIndex = tind;
        _data->vertPtr->tilingFactor = tilingFactor;
        _data->vertPtr++;

        _data->vertPtr->position = position + vec3(1.f, 1.f, 0.f);
        _data->vertPtr->color = color;
        _data->vertPtr->uv = {1.f, 1.f};
        _data->vertPtr->textureIndex = tind;
        _data->vertPtr->tilingFactor = tilingFactor;
        _data->vertPtr++;

        _data->vertPtr->position = position + vec3(0.f, 1.f, 0.f);
        _data->vertPtr->color = color;
        _data->vertPtr->uv = {0.f, 1.f};
        _data->vertPtr->textureIndex = tind;
        _data->vertPtr->tilingFactor = tilingFactor;
        _data->vertPtr++;

        _data->indexCount += 6;

        /*
        static const vec3 _z_axis = vec3(0.f, 0.f, 1.f);
        mat4 transform(1.f);
        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, glm::radians(rotation), _z_axis);
        transform = glm::scale(transform, vec3(scale.x, scale.y, 1.f));

        _data->textureShader->setUniform_Mat4("u_transform", transform);
        _data->textureShader->setUniform_vec4("u_color", color);
        _data->textureShader->setUniform_f32("u_tilingF", tilingFactor);

        if(texture != NULL) texture->bind();
        else _data->whiteTexture->bind();

        RenderAPI::drawIndexed(_data->vertArray);
        */
    }

    void Renderer2D::drawQuad(vec2 position, vec2 scale, f32 rotation,
            vec4 color, ref<Texture> texture, f32 tilingFactor){
        drawQuad(vec3(position.x, position.y, 0.f), scale, rotation, color, texture, tilingFactor);
    }

    void Renderer2D::drawQuad(const QuadProps& props){
        drawQuad(props.position, props.scale, props.rotation, props.color, props.texture, props.tilingFactor);
    }
}
