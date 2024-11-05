#include "towerdefense.h"

TowerDefense::TowerDefense(const char* name)
    :Layer(name){

    f32 verts[] = {
        //-0.5f, -0.5f, -0.5f,
        // 0.5f, -0.5f, -0.5f,
        //-0.5f,  0.5f, -0.5f,
        // 0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f, 0.f, 0.f,
         0.5f, -0.5f,  0.5f, 1.f, 0.f,
        -0.5f,  0.5f,  0.5f, 0.f, 1.f,
         0.5f,  0.5f,  0.5f, 1.f, 1.f,
    };

    u32 inds[] = {
        0, 1, 2, 1, 3, 2, // front
        //1, 5, 3, 5, 7, 3, // right
        //5, 4, 7, 4, 6, 7, // back
        //4, 0, 6, 0, 2, 6, // left
        //2, 3, 6, 3, 7, 6, // top
        //4, 5, 0, 5, 1, 0, // bottom
    };

    BufferLayout layout {
        {ShaderDataType::T_vec3, "aPos"},
        {ShaderDataType::T_vec2, "aUV"},
    };

    _quad = shareable<VAO>();
    _shader = shareable<Shader>("./assets/shaders/shader.glsl");
    _logo = shareable<Texture>("./assets/textures/test.png");
    _tiles = shareable<Texture>("./assets/textures/test2.png");


    ref<VertexBuffer> vb = shareable<VertexBuffer>(verts, sizeof(verts));
    vb->setLayout(layout);
    ref<IndexBuffer> ib = shareable<IndexBuffer>(inds, sizeof(inds)); 

    _quad->addVertexBuffer(vb);
    _quad->addIndexBuffer(ib);

    _cam.setPosition(vec3(0.f, 0.f, 3.f));
    _cam.lookAt(vec3(0.f, 0.f, 0.f));

    _shader->bind();
    _shader->setUniform_i32("u_texture", 0);
}

void TowerDefense::handleEvents(){
    static float moveSpeed = 10.f;
    vec3 camPos = _cam.getPosition();

    if(Input::getKeyDown(Everest::K_w)){
        camPos.z -= moveSpeed * Time::getDeltaTime();
    }
    if(Input::getKeyDown(Everest::K_s)){
        camPos.z += moveSpeed * Time::getDeltaTime();
    }
    if(Input::getKeyDown(Everest::K_a)){
        camPos.x -= moveSpeed * Time::getDeltaTime();
    }
    if(Input::getKeyDown(Everest::K_d)){
        camPos.x += moveSpeed * Time::getDeltaTime();
    }
    if(Input::getKeyDown(Everest::K_q)){
        camPos.y += moveSpeed * Time::getDeltaTime();
    }
    if(Input::getKeyDown(Everest::K_e)){
        camPos.y -= moveSpeed * Time::getDeltaTime();
    }

    _cam.setPosition(camPos);
    _cam.lookAt(vec3(0.f, 0.f, 0.f));
}

void TowerDefense::onUpdate(){
    this->handleEvents();

    Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
    Renderer::issue_clear();
    Renderer::beginScene(_cam);

    vec3 position(0), color(0.f, 0.6f, 0.6f);
    //position.x = glm::sin(Time::getDeltaTime());
    _logo->bind();
    Renderer::submit(_quad, _shader, glm::translate(mat4(1.f), position));
    position.z = 1.f;
    _tiles->bind();
    Renderer::submit(_quad, _shader, glm::translate(mat4(1.f), position));
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
