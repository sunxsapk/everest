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

void main() {
    float dist = sqrt(dot(_normCoord, _normCoord));
    float ivalue = step(1.f-_thickness, dist) * step(dist, 1.f);
    color0 = _color * ivalue;
    entityID = _id;
}



