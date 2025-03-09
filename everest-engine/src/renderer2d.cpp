#include "renderer/renderer2d.h"
#include "core/core.h"
#include "math/utils.h"

namespace Everest {
    Renderer2Ddata* Renderer2D::_data = new Renderer2Ddata;
    RendererStats Renderer2D::_stats;

    void Renderer2D::init(){
        EV_profile_function();

        setLineWidth(2.f);

        _data->quadVertArray = createRef<VAO>();
        _data->quadVertBase = new QuadVertex[_data->maxVertices];
        _data->quadVertPtr = _data->quadVertBase;

        _data->circleVertArray = createRef<VAO>();
        _data->circleVertBase = new CircleVertex[_data->maxVertices];
        _data->circleVertPtr = _data->circleVertBase;

        _data->lineVertArray = createRef<VAO>();
        _data->lineVertBase = new LineVertex[_data->maxVertices];
        _data->lineVertPtr = _data->lineVertBase;

        _data->quadVertPos[0] = vec4(-0.5f, -0.5f, 0.f, 1.f);
        _data->quadVertPos[1] = vec4( 0.5f, -0.5f, 0.f, 1.f);
        _data->quadVertPos[2] = vec4( 0.5f,  0.5f, 0.f, 1.f);
        _data->quadVertPos[3] = vec4(-0.5f,  0.5f, 0.f, 1.f);

        ref<VertexBuffer> qvb = createRef<VertexBuffer>(_data->maxVertices * sizeof(QuadVertex));
        BufferLayout quadDataLayout {
            {ShaderDataType::T_vec4, "color"},
            {ShaderDataType::T_vec3, "position"},
            {ShaderDataType::T_vec2, "uv"},
            {ShaderDataType::T_float, "textureIndex"},
#ifdef EDITOR_BUILD
            {ShaderDataType::T_int, "id"},
#endif
        };
        qvb->setLayout(quadDataLayout);
        _data->quadVertArray->addVertexBuffer(qvb);

        ref<VertexBuffer> cvb = createRef<VertexBuffer>(_data->maxVertices * sizeof(CircleVertex));
        BufferLayout circleDataLayout {
            {ShaderDataType::T_vec4, "color"},
            {ShaderDataType::T_vec3, "position"},
            {ShaderDataType::T_vec2, "normCoord"},
            {ShaderDataType::T_vec2, "uv"},
            {ShaderDataType::T_float, "textureIndex"},
            {ShaderDataType::T_float, "thickness"},
            {ShaderDataType::T_float, "fade"},
#ifdef EDITOR_BUILD
            {ShaderDataType::T_int, "id"},
#endif
        };
        cvb->setLayout(circleDataLayout);
        _data->circleVertArray->addVertexBuffer(cvb);

        ref<VertexBuffer> lvb = createRef<VertexBuffer>(_data->maxVertices * sizeof(LineVertex));
        BufferLayout lineDataLayout {
            {ShaderDataType::T_vec3, "position"},
            {ShaderDataType::T_vec4, "color"},
        };
        lvb->setLayout(lineDataLayout);
        _data->lineVertArray->addVertexBuffer(lvb);

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
        _data->quadVertArray->addIndexBuffer(qib);
        _data->circleVertArray->addIndexBuffer(qib);
        delete[] qind;

        _data->whiteTexture = createRef<Texture>(uvec2(1));
        u32 whiteText_data = 0xffffffff;
        _data->whiteTexture->setData(&whiteText_data, 4);

#ifdef EDITOR_BUILD
        _data->quadShader = createRef<Shader>(_getEngineAssetsPath("shaders/editor_quadShader.glsl"));
        _data->circleShader = createRef<Shader>(_getEngineAssetsPath("shaders/editor_circleShader.glsl"));
        _data->lineShader = createRef<Shader>(_getEngineAssetsPath("shaders/lineShader.glsl"));
#else
        _data->quadShader = createRef<Shader>(_getEngineAssetsPath("shaders/quadShader.glsl"));
        _data->circleShader = createRef<Shader>(_getEngineAssetsPath("shaders/circleShader.glsl"));
        _data->lineShader = createRef<Shader>(_getEngineAssetsPath("shaders/lineShader.glsl"));
#endif
        i32 samplerArr[MAX_TEXTURES];
        for(i32 i=0; i<_data->maxTexSlots; i++) samplerArr[i] = i;
        _data->quadShader->bind();
        _data->quadShader->setUniform_iarr("u_textures", samplerArr, _data->maxTexSlots);
        _data->circleShader->bind();
        _data->circleShader->setUniform_iarr("u_textures", samplerArr, _data->maxTexSlots);

        _data->textures[0] = _data->whiteTexture;
        _data->texCount = 1;
    }

    void Renderer2D::quit(){
        EV_profile_function();

        delete[] _data->quadVertBase;
        delete[] _data->circleVertBase;
        delete[] _data->lineVertBase;
        delete _data;
    }

    void Renderer2D::beginScene(camera_c& camera, mat4 camTransform){
        EV_profile_function();

        glm::mat4 vpmat = camera.getProjection() * glm::inverse(camTransform);
        _data->quadShader->bind();
        _data->quadShader->setUniform_Mat4("u_vpmat", vpmat);

        _data->circleShader->bind();
        _data->circleShader->setUniform_Mat4("u_vpmat", vpmat);

        _data->lineShader->bind();
        _data->lineShader->setUniform_Mat4("u_vpmat", vpmat);

        _data->whiteTexture->bind();

        _data->quadIndexCount = 0;
        _data->circleIndexCount = 0;
        _data->lineCount = 0;
        _data->texCount = 1; //white texture
        _data->quadVertPtr = _data->quadVertBase;
        _data->circleVertPtr = _data->circleVertBase;
        _data->lineVertPtr = _data->lineVertBase;

        _stats = {0,0,0,0,0};
    }

    void Renderer2D::endScene(){
        EV_profile_function();

        flushTextured();
        flushLines();
    }

    void Renderer2D::flushTextured(){
        u32 datasize = (u8*)_data->quadVertPtr - (u8*)_data->quadVertBase;
        if(datasize != 0){
            _data->quadVertArray->getVertexBuffer()->setData(_data->quadVertBase, datasize);

            for(u32 i=0; i<_data->texCount; i++){
                _data->textures[i]->bind(i);
            }

            _data->quadShader->bind();
            RenderAPI::drawIndexed(_data->quadVertArray, _data->quadIndexCount);
            _stats.drawCalls++;
        }

        datasize = (u8*)_data->circleVertPtr - (u8*)_data->circleVertBase;
        if(datasize != 0){
            _data->circleVertArray->getVertexBuffer()->setData(_data->circleVertBase, datasize);

            for(u32 i=0; i<_data->texCount; i++){
                _data->textures[i]->bind(i);
            }

            _data->circleShader->bind();
            RenderAPI::drawIndexed(_data->circleVertArray, _data->circleIndexCount);
            _stats.drawCalls++;
        }

        _data->quadIndexCount = 0;
        _data->circleIndexCount = 0;
        _data->texCount = 1;
        _data->quadVertPtr = _data->quadVertBase;
        _data->circleVertPtr = _data->circleVertBase;
    }

    void Renderer2D::flushLines(){
        u32 datasize = (u8*)_data->lineVertPtr - (u8*)_data->lineVertBase;
        if(datasize != 0){
            _data->lineVertArray->getVertexBuffer()->setData(_data->lineVertBase, datasize);
            _data->lineShader->bind();
            RenderAPI::drawLines(_data->lineVertArray, _data->lineCount * 2);
            _stats.drawCalls++;
        }

        _data->lineCount = 0;
        _data->lineVertPtr = _data->lineVertBase;
    }

    void Renderer2D::drawLine(vec3 point1, vec3 point2, vec4 color){
        EV_profile_function();

        if(_data->lineCount == _data->maxLines) flushLines();

        _data->lineVertPtr->position = point1;
        _data->lineVertPtr->color = color;
        _data->lineVertPtr++;

        _data->lineVertPtr->position = point2;
        _data->lineVertPtr->color = color;
        _data->lineVertPtr++;

        _data->lineCount++;
        _stats.lineCount++;
        _stats.vertexCount+=2;
    }

    void Renderer2D::drawRect(mat4 transform, vec4 color){
        constexpr u32 quadVertCount = 4;

        vec3 coords[4];
        for(int i=0; i<quadVertCount; i++){
            coords[i] = transform * _data->quadVertPos[i];
        }

        for(int i=0; i < quadVertCount; i++){
            drawLine(coords[i], coords[(i+1)%4], color);
        }
    }

    void Renderer2D::drawRect(vec3 pos, f32 rotation, vec2 scale, vec4 color){
        constexpr u32 quadVertCount = 4;

        vec3 coords[4];
        for(int i=0; i<quadVertCount; i++){
            coords[i] = Math::transformOrtho(_data->quadVertPos[i], pos, scale, rotation);
        }

        for(int i=0; i < quadVertCount; i++){
            drawLine(coords[i], coords[(i+1)%4], color);
        }
    }

    void Renderer2D::drawQuad(vec3 position, vec2 scale, f32 rotation,
            vec4 color, ref<Texture> texture){
        EV_profile_function();

        i32 tind = 0;
        _checkTexture(tind, texture);

        if(_data->quadIndexCount == _data->maxIndices) flushTextured();

        constexpr vec2 uvs[] = {{0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}};
        constexpr u32 quadVertCount = 4;

        for(int i=0; i<quadVertCount; i++){
            //_data->quadVertPtr->position = transform * _data->quadVertPos[i];
            _data->quadVertPtr->position = Math::transformOrtho(_data->quadVertPos[i],
                    position, scale, glm::radians(rotation));
            _data->quadVertPtr->color = color;
            _data->quadVertPtr->uv = uvs[i];
            _data->quadVertPtr->textureIndex = tind;
            _data->quadVertPtr++;
        }
        _data->quadIndexCount += 6;

        _stats.quadCount++;
        _stats.vertexCount += 4;
    }

    void Renderer2D::drawCircle(vec3 position, f32 diameter, vec4 color, f32 thickness, f32 fade, ref<Texture> texture){
        EV_profile_function();

        i32 tind = 0;
        _checkTexture(tind, texture);

        if(_data->circleIndexCount == _data->maxIndices) flushTextured();

        constexpr u32 quadVertCount = 4;
        constexpr vec2 uvs[] = {{0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}};

        for(int i=0; i<quadVertCount; i++){
            //_data->circleVertPtr->position = transform * _data->quadVertPos[i];
            vec3 qvp = _data->quadVertPos[i];
            _data->circleVertPtr->position = qvp * diameter + position;
            _data->circleVertPtr->color = color;
            _data->quadVertPtr->uv = uvs[i];
            _data->quadVertPtr->textureIndex = tind;
            _data->circleVertPtr->normCoord = qvp * 2.f;
            _data->circleVertPtr->thickness = thickness;
            _data->circleVertPtr->fade = fade;
            _data->circleVertPtr++;
        }
        _data->circleIndexCount += 6;
        _stats.quadCount++;
        _stats.vertexCount += 4;
    }

    void Renderer2D::drawSprite(const spriteRenderer_c& sprite, vec3 position, vec2 scale, f32 rotation
#ifdef EDITOR_BUILD
            , i32 id
#endif
            ){
        EV_profile_function();

        i32 tind = 0;
        _checkTexture(tind, sprite.texture);

        if(_data->quadIndexCount == _data->maxIndices) flushTextured();

        constexpr u32 quadVertCount = 4;
        vec2 uvs[] = {sprite.startUV,
            sprite.startUV+vec2(sprite.sizeUV.x, 0.f),
            sprite.startUV + sprite.sizeUV,
            sprite.startUV+vec2(0.f, sprite.sizeUV.y)};

        for(int i=0; i<quadVertCount; i++){
            //_data->quadVertPtr->position = transform * _data->quadVertPos[i];
            _data->quadVertPtr->position = Math::transformOrtho(_data->quadVertPos[i],
                    position, scale, glm::radians(rotation));
            _data->quadVertPtr->color = sprite.color;
            _data->quadVertPtr->uv = uvs[i];
            _data->quadVertPtr->textureIndex = tind;
#ifdef EDITOR_BUILD
            _data->quadVertPtr->id = id;
#endif
            _data->quadVertPtr++;
        }
        _data->quadIndexCount += 6;

        _stats.quadCount++;
        _stats.vertexCount += 4;
    }

    void Renderer2D::drawSprite(mat4 transform, const spriteRenderer_c& sprite
#ifdef EDITOR_BUILD
            , i32 id
#endif
            ){
        EV_profile_function();

        i32 tind = 0;
        _checkTexture(tind, sprite.texture);

        if(_data->quadIndexCount == _data->maxIndices) flushTextured();

        constexpr u32 quadVertCount = 4;
        vec2 uvs[] = {sprite.startUV,
            sprite.startUV+vec2(sprite.sizeUV.x, 0.f),
            sprite.startUV + sprite.sizeUV,
            sprite.startUV+vec2(0.f, sprite.sizeUV.y)};

        for(int i=0; i<quadVertCount; i++){
            _data->quadVertPtr->position = transform * _data->quadVertPos[i];
            _data->quadVertPtr->color = sprite.color;
            _data->quadVertPtr->uv = uvs[i];
            _data->quadVertPtr->textureIndex = tind;
#ifdef EDITOR_BUILD
            _data->quadVertPtr->id = id;
#endif
            _data->quadVertPtr++;
        }
        _data->quadIndexCount += 6;

        _stats.quadCount++;
        _stats.vertexCount += 4;
    }

    void Renderer2D::drawCircle(mat4 transform, const circleRenderer_c& circle
#ifdef EDITOR_BUILD
            , i32 id
#endif
            ){
        EV_profile_function();

        i32 tind = 0;
        _checkTexture(tind, circle.texture);

        if(_data->circleIndexCount == _data->maxIndices) flushTextured();
        constexpr u32 quadVertCount = 4;

        vec2 uvs[] = {circle.startUV,
            circle.startUV+vec2(circle.sizeUV.x, 0.f),
            circle.startUV + circle.sizeUV,
            circle.startUV+vec2(0.f, circle.sizeUV.y)};


        for(int i=0; i<quadVertCount; i++){
            vec4& qvp = _data->quadVertPos[i];
            _data->circleVertPtr->position = transform * qvp;
            _data->circleVertPtr->color = circle.color;
            _data->circleVertPtr->uv = uvs[i];
            _data->circleVertPtr->textureIndex = tind;
            _data->circleVertPtr->normCoord = {qvp.x * 2.f, qvp.y * 2.f};
            _data->circleVertPtr->thickness = circle.thickness;
            _data->circleVertPtr->fade = circle.fade;
#ifdef EDITOR_BUILD
            _data->circleVertPtr->id = id;
#endif
            _data->circleVertPtr++;
        }
        _data->circleIndexCount += 6;
        _stats.quadCount++;
        _stats.vertexCount += 4;
    }

    void Renderer2D::drawCircle(mat4 transform, vec4 color, f32 thickness, f32 fade
#ifdef EDITOR_BUILD
            , i32 id
#endif
            ){
        EV_profile_function();

        if(_data->circleIndexCount == _data->maxIndices) flushTextured();
        constexpr u32 quadVertCount = 4;
        for(int i=0; i<quadVertCount; i++){
            vec4& qvp = _data->quadVertPos[i];
            _data->circleVertPtr->position = transform * qvp;
            _data->circleVertPtr->color = color;
            _data->circleVertPtr->normCoord = {qvp.x * 2.f, qvp.y * 2.f};
            _data->circleVertPtr->thickness = thickness;
            _data->circleVertPtr->fade = fade;
#ifdef EDITOR_BUILD
            _data->circleVertPtr->id = id;
#endif
            _data->circleVertPtr++;
        }
        _data->circleIndexCount += 6;
        _stats.quadCount++;
        _stats.vertexCount += 4;
    }

    void Renderer2D::_checkTexture(i32& tind, ref<Texture> tex){
        if(tex == nullptr) return;

        for(u32 i=1; i<_data->texCount; i++){
            if(*_data->textures[i].get() == *tex.get()){
                tind = i;
                break;
            }
        }
        if(tind == 0){
            if(_data->texCount == Renderer2Ddata::maxTexSlots) flushTextured();
            tind = _data->texCount;
            _data->textures[_data->texCount++] = tex;
            _stats.textureCount++;
        }
    }

}
