#version 330 core
layout (location = 0) in vec2 pos;

void main() {
	gl_Position = vec4(pos.x / 30.0, pos.y / 30.0, 0.0, 1.0);
}