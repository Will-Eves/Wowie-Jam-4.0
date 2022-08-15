#version 410 core

layout(location = 0) in vec2 v_Position;
layout(location = 1) in vec2 v_Texture;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec2 albedoCoordinate;
out vec2 fragmentPosition;

void main() {
	albedoCoordinate = v_Texture;
	fragmentPosition = vec2(modelMatrix * vec4(v_Position, 0.0, 1.0));

	gl_Position = projectionMatrix * modelMatrix * vec4(v_Position, 0.0, 1.0);
}