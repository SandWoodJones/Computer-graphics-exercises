#version 330 core

out vec4 color;

uniform int isWall;

void main() {
	if (isWall == 1) {
		color = vec4(0.35, 0.96, 0.3, 1.0);
	} else {
		color = vec4(1.0, 1.0, 0.0, 1.0);
	}
}