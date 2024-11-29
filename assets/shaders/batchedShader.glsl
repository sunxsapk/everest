#type vertex
#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in float textureInd;
layout (location = 4) in float tilingFactor;

uniform mat4 u_viewProjMat;

out vec4 _color;
out vec2 _uv;
out float _textureInd;
out float _tilingFactor;

void main() {
    gl_Position = u_viewProjMat * vec4(position, 1.0);

    _color = color;;
    _uv = uv;
    _textureInd = textureInd;
    _tilingFactor = tilingFactor;
}


#type fragment
#version 420 core

out vec4 fragmentColor;

in vec4 _color;
in vec2 _uv;
in float _textureInd;
in float _tilingFactor;

uniform sampler2D u_textures[32];

void main() {
    int index = int(round(_textureInd));
    vec4 cl = texture(u_textures[index], _uv * _tilingFactor);
    fragmentColor = cl * _color;
}

