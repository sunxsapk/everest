#include "gizmos.h"
#include "scenepanel.h"
#include "sceneheirarchy.h"

#include "ImGuizmo.h"

namespace Everest {

    bool Gizmos::showGizmos = true;
    bool Gizmos::showPhysicsShapes = false;
    Gizmos *Gizmos::_instance = nullptr;
    ImGuizmo::OPERATION Gizmos::operation = ImGuizmo::OPERATION::TRANSLATE;
    bool Gizmos::isLocalTransform = true;

    void Gizmos::initGrid(){
        _instance->gridShader = createRef<Shader>(_getEngineAssetsPath("shaders/gridShader.glsl"));
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

        bool is2D = camera.camera.is2d();
        vec3 _sz = is2D? vec3(size.x, size.y, -100.f) : vec3(size.x, 0.f, size.y);
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
                    camera.camera.get_lenssize()));
        _instance->gridShader->setUniform_i32("u_is2D", is2D);
        RenderAPI::drawIndexed(_instance->gridVertexArray, 6);

        /*float gridSize = 10.0f;
        float gridSpacing = 1.0f;

        static mat4 im = glm::mat4(1.f);
        ImGuizmo::DrawGrid(
                glm::value_ptr(camera.getView()),
                glm::value_ptr(camera.camera.getProjection()),
                glm::value_ptr(im),
                gridSize
            );*/
    }

    void Gizmos::renderTransformationGizmo(EditorCamera& cam){
        if(!showGizmos) return;
        Entity ent = SceneHeirarchyUI::getSelectedEntity();
        if(!ent.isValid()) return;
        ImGuizmo::SetOrthographic(cam.camera.is2d());
        ImGuizmo::SetDrawlist();
        ImVec2 pos = ImGui::GetItemRectMin();
        ImVec2 end = ImGui::GetItemRectMax();
        ImGuizmo::SetRect(pos.x, pos.y, end.x - pos.x, end.y - pos.y);

        bool snap = Input::getKeyDown(K_left_control);
        f32 snapValue = operation == ImGuizmo::ROTATE? 45.f : 0.25f;
        f32 snapVals[3] = {snapValue, snapValue, snapValue};

        auto& transform = ent.get<transform_c>();
        mat4 tmat = transform;
        ImGuizmo::Manipulate(glm::value_ptr(cam.getView()), glm::value_ptr(cam.camera.getProjection()),
                operation, isLocalTransform?ImGuizmo::LOCAL : ImGuizmo::WORLD, glm::value_ptr(tmat),
                nullptr, snap ? snapVals : nullptr);

        if(ImGuizmo::IsUsing()){
            Math::decomposeTransform(transform, tmat);
        } else if(ScenePanel::isFocused() && Input::mouseButtonUp(M_1)) {
            if(Input::getKeyDown(K_w)){
                Gizmos::operation = ImGuizmo::TRANSLATE;
            } else if(Input::getKeyDown(K_e)){
                Gizmos::operation = ImGuizmo::SCALE;
            } else if(Input::getKeyDown(K_r)){
                Gizmos::operation = ImGuizmo::ROTATE;
            }
        }
    }
}

