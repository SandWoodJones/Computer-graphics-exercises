#version 330 core
layout (location = 0) in vec2 pos;

void main() {
	gl_Position = vec4(pos.x / 4.5 , pos.y / 2.5, 0.0, 1.0);
}