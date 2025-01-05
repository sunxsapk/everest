#type vertex
#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 normCoord;
layout (location = 3) in float thickness;
layout (location = 4) in float fade;
layout (location = 5) in int id;

uniform mat4 u_vpmat;

out vec4 _color;
out vec2 _normCoord;
out float _thickness;
out float _fade;
flat out int _id;

void main() {
    gl_Position = u_vpmat * vec4(position, 1.0);

    _color = color;;
    _normCoord = normCoord;
    _thickness = thickness;
    _fade = fade;
    _id = id;
}


#type fragment
#version 400 core

layout(location = 0) out vec4 color0;
layout(location = 1) out int entityID;

in vec4 _color;
in vec2 _normCoord;
in float _thickness;
in float _fade;
flat in int _id;

float circlePixCalc(float d){
    d = 1.f - d;
    float fd = _fade + 0.0001f; //smoothstep returns 0 when _fade is exactly 0
    float x = smoothstep(0.f, fd, d) * smoothstep(_thickness+fd, _thickness, d);
    return x;
}

void main() {
    float dis = length(_normCoord);
    if(dis > 1.f) discard;
    float c = circlePixCalc(dis);
    color0 = _color * c;
    entityID = _id;
}



