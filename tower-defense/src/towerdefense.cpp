#include "towerdefense.h"

TowerDefense::TowerDefense(const char* name)
    :Layer(name){
    const char* vsh = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;

        uniform mat4 u_viewProjMat;
        out vec3 fColor;

        void main() {
            gl_Position = u_viewProjMat * vec4(aPos, 1.0);
            fColor = aColor;
        }
    )";

    const char* fsh = R"(
        #version 330 core
        out vec4 FragColor;
        in vec3 fColor;
        
        void main() {
            FragColor = vec4(fColor, 1.0);
        }
    )";

    f32 verts[] = {
        -1.0f, -1.0f, 1.0f,  0.f, 0.f, 0.f,
         1.0f, -1.0f, 1.0f,  1.f, 0.f, 0.f,
         1.0f,  1.0f, 1.0f,  0.f, 1.f, 0.f,
        -1.0f,  1.0f, 1.0f,  0.f, 0.f, 1.f,
    };

    u32 inds[] = {
        0, 1, 2,
        2, 3, 0
    };

    BufferLayout layout {
        {T_vec3, "aPos"},
        {T_vec3, "aColor"},
    };

    _quad = std::make_shared<VAO>();
    _shader = (std::make_shared<Shader>(vsh, fsh));

    p_shared(VertexBuffer) vb = std::make_shared<VertexBuffer>(verts, sizeof(verts));
    vb->setLayout(layout);
    p_shared(IndexBuffer) ib = std::make_shared<IndexBuffer>(inds, sizeof(inds)); 

    _quad->addVertexBuffer(vb);
    _quad->addIndexBuffer(ib);

    _cam.setRotation(vec3(0.f, 90.f, 0.f));
    _cam.setPosition(vec3(0.f, 0.f, 3.f));
}

void TowerDefense::handleEvents(){
    static float rotSpeed = 10.f;
    vec3 camPos = _cam.getPosition();
    vec3 camRot = _cam.getRotation();

    if(Input::getKeyDown(Everest::K_left_shift)){
        if(Input::getKeyDown(Everest::K_w)){
            camRot.z -= rotSpeed * Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_s)){
            camRot.z += rotSpeed * Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_a)){
            camRot.x -= rotSpeed * Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_d)){
            camRot.x += rotSpeed * Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_q)){
            camRot.y += rotSpeed * Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_e)){
            camRot.y -= rotSpeed * Time::getDeltaTime();
        }
    }else{
        if(Input::getKeyDown(Everest::K_w)){
            camPos.z -= Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_s)){
            camPos.z += Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_a)){
            camPos.x -= Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_d)){
            camPos.x += Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_q)){
            camPos.y += Time::getDeltaTime();
        }
        if(Input::getKeyDown(Everest::K_e)){
            camPos.y -= Time::getDeltaTime();
        }
    }

    _cam.setPosition(camPos);
    _cam.setRotation(camRot);
}

void TowerDefense::onUpdate(){
    this->handleEvents();

    Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
    Renderer::issue_clear();
    Renderer::beginScene(_cam);
    Renderer::submit(_quad, _shader);
    Renderer::endScene();
}

void TowerDefense::onDebugRender(){
    ImGui::Begin(this->_name);
    ImGui::Text("%s", _evLog);
    ImGui::End();
}

void TowerDefense::onEvent(Event& event){
    sprintf(this->_evLog, "%s", event.toString().c_str());
}
