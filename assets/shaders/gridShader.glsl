#type vertex
#version 420 core
layout (location = 0) in vec3 position;

uniform mat4 u_vpmat;

out vec3 pxPos;

void main() {
    gl_Position = u_vpmat * vec4(position, 1.0f);
    pxPos = position;
}


#type fragment
#version 420 core

#define THICKNESS 0.02f
#define AXIS_COLOR vec4(1.f, 0.f, 0.f, 1.f)
#define THICK_COLOR vec4(1.f, 1.f, 1.f, 1.f)
#define THIN_COLOR vec4(.4f, .4f, .4f, 1.f)


uniform vec3 u_camera;

out vec4 FragColor;

in vec3 pxPos;

bool onEdge(float p){
    return abs(p) < THICKNESS;
}

float th(float x){
    return smoothstep(THICKNESS, 0, abs(x));
}

float sm(float x){
    return smoothstep(THICKNESS, 0, abs(x+0.5f));
}

float getFogFc(float d){
    const float _MD = 5.f;
    if(d < _MD) return 1.f;
    return 1.f/ (d - _MD + 0.5f);
}

void main(){
    vec3 fp = pxPos - round(pxPos);

    if(onEdge(fp.x)) FragColor = THICK_COLOR * th(fp.x);
    else if(onEdge(fp.z)) FragColor = THICK_COLOR * th(fp.z);
    else if(onEdge(fp.x + 0.5f)) FragColor = THIN_COLOR * sm(fp.x);
    else if(onEdge(fp.z + 0.5f)) FragColor = THIN_COLOR * sm(fp.z);
    else {
        FragColor = vec4(0.f);
        return;
    }
    if(onEdge(pxPos.x) || onEdge(pxPos.z)) FragColor = AXIS_COLOR;

    float d = distance(u_camera, pxPos);
    FragColor.a *= getFogFc(d);
}
