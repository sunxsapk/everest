#type vertex
#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 normCoord;
layout (location = 3) in float thickness;
layout (location = 4) in float fade;

uniform mat4 u_vpmat;

out vec4 _color;
out vec2 _normCoord;
out float _thickness;
out float _fade;

void main() {
    gl_Position = u_vpmat * vec4(position, 1.0);

    _color = color;;
    _normCoord = normCoord;
    _thickness = thickness;
    _fade = fade;
}


#type fragment
#version 400 core

layout(location = 0) out vec4 color0;

in vec4 _color;
in vec2 _normCoord;
in float _thickness;
in float _fade;

float circlePixCalc(){
    float d = 1.f - length(_normCoord);
    float fd = _fade + 0.0001f; //smoothstep returns 0 when _fade is exactly 0
    float x = smoothstep(0.f, fd, d) * smoothstep(_thickness+fd, _thickness, d);
    return x;
}

void main() {
    color0 = _color * circlePixCalc();
}



