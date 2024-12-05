#include "gizmos.h"


namespace Everest {

    Shader *Gizmos::_gridShader = nullptr;
    vec3 Gizmos::_gridVertices[4]{};
    ref<VAO> Gizmos::_vao;

    void Gizmos::init(){
        _gridShader = new Shader("assets/shaders/gridShader.glsl");
        _vao = createRef<VAO>();
         
        ref<VertexBuffer> gvb = createRef<VertexBuffer>(4 * sizeof (vec3));
        BufferLayout layout {
            {ShaderDataType::T_vec3, "position"},
        };
        gvb->setLayout(layout);
        _vao->addVertexBuffer(gvb);

        u32 qind[6] { 0, 1, 2, 2, 3, 0 };
        ref<IndexBuffer> qib = createRef<IndexBuffer>(qind, sizeof(qind));
        _vao->addIndexBuffer(qib);
    }

    void Gizmos::quit(){
        delete _gridShader;
        _vao.reset();
    }

    void Gizmos::renderGrid(EditorCamera& camera, vec2 size, bool is2D){
        vec3 _sz = is2D? vec3(size.x, size.y, 0.f) : vec3(size.x, 0.f, size.y);
        _gridVertices[0] = vec3(-_sz.x, -_sz.y, -_sz.z);
        _gridVertices[1] = vec3( _sz.x, -_sz.y, -_sz.z);
        _gridVertices[2] = vec3( _sz.x,  _sz.y,  _sz.z);
        _gridVertices[3] = vec3(-_sz.x,  _sz.y,  _sz.z);
        _vao->getVertexBuffer()->setData(_gridVertices, sizeof(_gridVertices));

        _gridShader->bind();
        _gridShader->setUniform_Mat4("u_vpmat", camera.getVPmat());
        _gridShader->setUniform_vec3("u_camera", camera.transform.position);
        RenderAPI::drawIndexed(_vao, 6);
    }
}
