#version 330 core
layout (location = 0) in vec2 pos;

void main() {
	gl_Position = vec4(pos.x / 1.7 , pos.y / 1.05, 0.0, 1.0);
}