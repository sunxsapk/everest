#include "renderer/renderer2d.h"
#include "renderer/renderAPI.h"

namespace Everest {
    Renderer2Ddata* Renderer2D::_data = new Renderer2Ddata;
    RendererStats Renderer2D::_stats;

    void Renderer2D::init(){
        EV_profile_function();

        _data->vertArray = createRef<VAO>();
        _data->vertBase = new QuadVertex[_data->maxVertices];
        _data->vertPtr = _data->vertBase;

        _data->quadVertPos[0] = vec3(-0.5f, -0.5f, 0.f);
        _data->quadVertPos[1] = vec3( 0.5f, -0.5f, 0.f);
        _data->quadVertPos[2] = vec3( 0.5f,  0.5f, 0.f);
        _data->quadVertPos[3] = vec3(-0.5f,  0.5f, 0.f);

        ref<VertexBuffer> qvb = createRef<VertexBuffer>(_data->maxVertices * sizeof(QuadVertex));
        BufferLayout layout {
            {ShaderDataType::T_vec3, "position"},
            {ShaderDataType::T_vec4, "color"},
            {ShaderDataType::T_vec2, "uv"},
            {ShaderDataType::T_float, "textureIndex"},
            {ShaderDataType::T_float, "tilingFactor"},
        };
        qvb->setLayout(layout);
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

        _data->whiteTexture = createRef<Texture>(uvec2(1));
        u32 whiteText_data = 0xffffffff;
        _data->whiteTexture->setData(&whiteText_data, 4);

        _data->textureShader = createRef<Shader>("assets/shaders/batchedShader.glsl");
        i32 samplerArr[32];
        for(i32 i=0; i<_data->maxTexSlots; i++) samplerArr[i] = i;
        _data->textureShader->bind();
        _data->textureShader->setUniform_iarr("u_textures", samplerArr, _data->maxTexSlots);

        _data->textures[0] = _data->whiteTexture;
        _data->texCount = 1;
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

        _stats = {0,0,0,0};
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

        _stats.drawCalls++;

        _data->indexCount = 0;
        _data->texCount = 1;
        _data->vertPtr = _data->vertBase;
    }

    void Renderer2D::drawQuad(vec3 position, vec2 scale, f32 rotation,
            vec4 color, ref<Texture> texture, f32 tilingFactor){
        EV_profile_function();

        i32 tind = 0;
        if(texture != NULL){
            for(u32 i=1; i<_data->texCount; i++){
                if(*_data->textures[i].get() == *texture.get()){
                    tind = i;
                    break;
                }
            }
            if(tind == 0){
                tind = _data->texCount;
                _data->textures[_data->texCount++] = texture;
                _stats.textureCount++;
            }
        }

        if(_data->indexCount == _data->maxIndices) flush();

        constexpr vec2 uvs[] = {{0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}};
        constexpr u32 quadVertCount = 4;

        for(int i=0; i<quadVertCount; i++){
            //_data->vertPtr->position = transform * _data->quadVertPos[i];
            _data->vertPtr->position = transformOrtho(_data->quadVertPos[i],
                    position, scale, glm::radians(rotation));
            _data->vertPtr->color = color;
            _data->vertPtr->uv = uvs[i];
            _data->vertPtr->textureIndex = tind;
            _data->vertPtr->tilingFactor = tilingFactor;
            _data->vertPtr++;
        }
        _data->indexCount += 6;

        _stats.quadCount++;
        _stats.vertexCount += 4;
    }
    void Renderer2D::drawSprite(Sprite sprite, vec3 position, vec2 scale, f32 rotation,
            vec4 color){
        EV_profile_function();

        i32 tind = 0;
        if(sprite.texture != NULL){
            for(u32 i=1; i<_data->texCount; i++){
                if(*_data->textures[i].get() == *sprite.texture.get()){
                    tind = i;
                    break;
                }
            }
            if(tind == 0){
                tind = _data->texCount;
                _data->textures[_data->texCount++] = sprite.texture;
                _stats.textureCount++;
            }
        }

        if(_data->indexCount == _data->maxIndices) flush();

        constexpr f32 tilingFactor = 1.f;
        constexpr u32 quadVertCount = 4;
        vec2 uvs[] = {sprite.startUV,
            sprite.startUV+vec2(sprite.sizeUV.x, 0.f),
            sprite.startUV + sprite.sizeUV,
            sprite.startUV+vec2(0.f, sprite.sizeUV.y)};

        for(int i=0; i<quadVertCount; i++){
            //_data->vertPtr->position = transform * _data->quadVertPos[i];
            _data->vertPtr->position = transformOrtho(_data->quadVertPos[i],
                    position, scale, glm::radians(rotation));
            _data->vertPtr->color = color;
            _data->vertPtr->uv = uvs[i];
            _data->vertPtr->textureIndex = tind;
            _data->vertPtr->tilingFactor = tilingFactor;
            _data->vertPtr++;
        }
        _data->indexCount += 6;

        _stats.quadCount++;
        _stats.vertexCount += 4;
    }


    void Renderer2D::drawQuad(vec2 position, vec2 scale, f32 rotation,
            vec4 color, ref<Texture> texture, f32 tilingFactor){
        drawQuad(vec3(position.x, position.y, 0.f), scale, rotation, color, texture, tilingFactor);
    }

    void Renderer2D::drawQuad(const QuadProps& props){
        drawQuad(props.position, props.scale, props.rotation, props.color, props.texture, props.tilingFactor);
    }
}
