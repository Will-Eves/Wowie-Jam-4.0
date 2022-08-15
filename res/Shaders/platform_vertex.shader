#version 410 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec2 v_Texture;
layout(location = 2) in vec3 v_Normal;
layout(location = 3) in vec3 v_Tangent;
layout(location = 4) in vec3 v_Bitangent;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 aCord;

// PBR Stuff
out vec3 fragmentPosition;
out vec3 normal;

void main() {
	aCord = v_Texture;

	// Set PBR Vars
	fragmentPosition = vec3(modelMatrix * vec4(v_Position, 1.0));
	normal = mat3(transpose(inverse(modelMatrix))) * v_Normal;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(v_Position, 1.0);
}