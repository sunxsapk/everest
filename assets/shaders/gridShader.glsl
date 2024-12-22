#type vertex
#version 400 core
layout (location = 0) in vec3 position;

uniform mat4 u_vpmat;

out vec3 pxPos;

void main() {
    gl_Position = u_vpmat * vec4(position, 1.0f);
    pxPos = position;
}


#type fragment
#version 400 core

#define THICKNESS 0.02f
#define AXIS_COLOR vec4(1.f, 0.f, 0.f, 1.f)
#define THICK_COLOR vec4(1.f, 1.f, 1.f, 1.f)
#define THIN_COLOR vec4(.4f, .4f, .4f, 1.f)
#define ACT_VIEW_D 4.f


uniform vec4 u_camera;
uniform int u_is2D;

layout(location = 0) out vec4 color0;
layout(location = 1) out int entityID;


in vec3 pxPos;

bool isThick(float p){
    return abs(p) < THICKNESS;
}

bool isThin(float p){
    return abs(p) < THICKNESS/4.f;
}

float th(float x){
    return smoothstep(THICKNESS, 0, abs(x));
}

float getFogFc(float d){
    if(d < ACT_VIEW_D) return 0.6f;
    return 0.6f/ (d - ACT_VIEW_D + 0.01f);
}

void main(){
    entityID = -1;
    vec3 fp = pxPos - round(pxPos);

    if(isThick(fp.x)) color0 = THICK_COLOR * th(fp.x);
    else if(isThin(fp.x + 0.5f)) color0 = THIN_COLOR;
    else if(u_is2D != 0){
        if(isThick(fp.y)) color0 = THICK_COLOR * th(fp.y);
        else if(isThin(fp.y + 0.5f)) color0 = THIN_COLOR;
        else {
            color0 = vec4(0.f);
            return;
        }
    } else if(isThick(fp.z)) color0 = THICK_COLOR * th(fp.z);
    else if(isThin(fp.z + 0.5f)) color0 = THIN_COLOR;
    else {
        color0 = vec4(0.f);
        return;
    }

    if(isThick(pxPos.x) || isThick(u_is2D == 0? pxPos.z : pxPos.y)) color0 = AXIS_COLOR;

    if(u_is2D != 0) color0.a = 0.6f;
    else {
        //float d = distance(u_camera.xyz, pxPos);
        float d = distance(u_camera.xz, pxPos.xz); // ignore cam-height
        color0.a *= getFogFc(d);
    }
}

