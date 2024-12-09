#include "gizmos.h"
#include "scenepanel.h"
#include "sceneheirarchy.h"

#include "ImGuizmo.h"

namespace Everest {

    bool Gizmos::showGizmos = true;
    Gizmos *Gizmos::_instance = nullptr;
    ImGuizmo::OPERATION Gizmos::operation = ImGuizmo::OPERATION::TRANSLATE;

    void Gizmos::initGrid(){
        _instance->gridShader = createRef<Shader>("assets/shaders/gridShader.glsl");
        _instance->gridVertexArray = createRef<VAO>();
         
        ref<VertexBuffer> gvb = createRef<VertexBuffer>(4 * sizeof (vec3));
        BufferLayout layout {
            {ShaderDataType::T_vec3, "position"},
        };
        gvb->setLayout(layout);
        _instance->gridVertexArray->addVertexBuffer(gvb);

        u32 qind[6] { 0, 1, 2, 2, 3, 0 };
        ref<IndexBuffer> qib = createRef<IndexBuffer>(qind, sizeof(qind));
        _instance->gridVertexArray->addIndexBuffer(qib);
    }

    void Gizmos::init(){
        _instance = new Gizmos();
        initGrid();
    }

    void Gizmos::quit(){
        delete _instance;
    }

    void Gizmos::renderGrid(EditorCamera& camera, vec2 size){
        if(!showGizmos) return;

        bool is2D = camera.is2D();
        vec3 _sz = is2D? vec3(size.x, size.y, 0.f) : vec3(size.x, 0.f, size.y);
        _sz /= 2.f;
        vec3 gridVertices[4]{
            vec3(-_sz.x, -_sz.y, -_sz.z),
            vec3( _sz.x, -_sz.y, -_sz.z),
            vec3( _sz.x,  _sz.y,  _sz.z),
            vec3(-_sz.x,  _sz.y,  _sz.z)
        };
        _instance->gridVertexArray->getVertexBuffer()->setData(gridVertices, sizeof(gridVertices));

        _instance->gridShader->bind();
        _instance->gridShader->setUniform_Mat4("u_vpmat", camera.getVPmat());
        _instance->gridShader->setUniform_vec4("u_camera", vec4(camera.transform.position,
                    camera.camera.getOrtho_size()));
        _instance->gridShader->setUniform_i32("u_is2D", is2D);
        RenderAPI::drawIndexed(_instance->gridVertexArray, 6);
    }

    void Gizmos::renderTransformationGizmo(EditorCamera& cam){
        if(!showGizmos) return;
        Entity ent = SceneHeirarchyUI::getSelectedEntity();
        if(!ent.isValid()) return;
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 sz = ImGui::GetWindowSize();
        ImGuizmo::SetRect(pos.x, pos.y, sz.x, sz.y);

        auto& transform = ent.get<transform_c>();
        mat4 tmat = transform;
        ImGuizmo::Manipulate(glm::value_ptr(cam.getView()), glm::value_ptr(cam.camera.getProjection()),
                operation, ImGuizmo::LOCAL, glm::value_ptr(tmat));

        if(ImGuizmo::IsUsing()){
            Math::decomposeTransform(transform, tmat); // TODO: some errors in decompose
        }
    }

    void Gizmos::testGizmos(EditorCamera& cam){
        vec2 mp = Input::mousePosition() - ScenePanel::getSceneOffset();
        vec3 pos = cam.screenToWorldPos(mp);
        if(!cam.is2D()){
            pos = 10.f * glm::normalize(pos - cam.transform.position);
            pos += cam.transform.position;
        }
        Renderer2D::drawQuad(pos, vec2(0.5f), 0.f, vec4(0.f, 1.f, 1.f, .6f));
    }
}
