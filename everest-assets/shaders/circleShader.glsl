#type vertex
#version 400 core
layout (location = 0) in vec4 color;
layout (location = 1) in vec3 position;
layout (location = 2) in vec2 normCoord;
layout (location = 3) in vec2 uv;
layout (location = 4) in float textureInd;
layout (location = 5) in float thickness;
layout (location = 6) in float fade;

uniform mat4 u_vpmat;

out vec4 _color;
out vec2 _normCoord;
out vec2 _uv;
out float _textureInd;
out float _thickness;
out float _fade;

void main() {
    gl_Position = u_vpmat * vec4(position, 1.0);

    _color = color;;
    _normCoord = normCoord;
    _uv = uv;
    _textureInd = textureInd;
    _thickness = thickness;
    _fade = fade;
}


#type fragment
#version 400 core

#define MAX_TEXS 16

layout(location = 0) out vec4 color0;

in vec4 _color;
in vec2 _normCoord;
in vec2 _uv;
in float _textureInd;
in float _thickness;
in float _fade;

uniform sampler2D u_textures[MAX_TEXS];

float circlePixCalc(float d){
    d = 1.f - d;
    float fd = _fade + 0.0001f; //smoothstep returns 0 when _fade is exactly 0
    float x = smoothstep(0.f, fd, d) * smoothstep(_thickness+fd, _thickness, d);
    return x;
}

void main() {
    float dis = length(_normCoord);
    if(dis > 1.f) discard;
    int index = int(round(_textureInd));
    vec4 cl = texture(u_textures[index], _uv);
    float c = circlePixCalc(dis);
    color0 = _color * cl * c;
}
