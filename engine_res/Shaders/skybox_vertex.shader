#version 330 core

layout(location = 0) in vec3 v_Position;

out vec3 fragmentPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(){
    fragmentPosition = v_Position;
    gl_Position = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(v_Position, 1.0);
}