#type vertex
#version 400 core
layout (location = 0) in vec4 color;
layout (location = 1) in vec3 position;
layout (location = 2) in vec2 uv;
layout (location = 3) in float textureInd;

uniform mat4 u_vpmat;

out vec4 _color;
out vec2 _uv;
out float _textureInd;

void main() {
    gl_Position = u_vpmat * vec4(position, 1.0);

    _color = color;;
    _uv = uv;
    _textureInd = textureInd;
}


#type fragment
#version 400 core

#define MAX_TEXS 16

layout(location = 0) out vec4 color0;

in vec4 _color;
in vec2 _uv;
in float _textureInd;

uniform sampler2D u_textures[MAX_TEXS];

void main() {
    int index = int(round(_textureInd));
    vec4 cl = texture(u_textures[index], _uv);
    color0 = cl * _color;
}

