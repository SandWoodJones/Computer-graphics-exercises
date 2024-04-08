#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
out vec3 vert_color;
void main() {
	vert_color  = col;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}