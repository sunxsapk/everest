#type vertex
#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

uniform mat4 u_vpmat;

out vec4 _color;

void main() {
    gl_Position = u_vpmat * vec4(position, 1.0);

    _color = color;;
}


#type fragment
#version 400 core

layout(location = 0) out vec4 color0;

in vec4 _color;

void main() {
    color0 = _color;
}




