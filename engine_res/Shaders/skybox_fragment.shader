#version 330 core

in vec3 fragmentPosition;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v){
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

out vec4 color;

void main(){
    vec2 uv = SampleSphericalMap(normalize(fragmentPosition));
    color = texture2D(equirectangularMap, uv);
}