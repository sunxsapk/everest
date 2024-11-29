#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 u_viewProjMat;
uniform mat4 u_transform;

out vec2 _uv;
void main() {
    gl_Position = u_viewProjMat * u_transform * vec4(aPos, 1.0);
    _uv = aUV;
}


#type fragment
#version 330 core

in vec2 _uv;
out vec4 fragmentColor;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_tilingF;

void main() {
    fragmentColor = u_color * texture(u_texture, _uv * u_tilingF);
}

