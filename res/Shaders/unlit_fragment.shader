#version 410 core

in vec2 albedoCoordinate;

struct Material {
	float alpha;

	vec3 albedo;
	vec2 albedoTiling;
	sampler2D albedoMap;
	int hasAlbedoMap;

	vec3 emissive;
	vec2 emissiveTiling;
	sampler2D emissiveMap;
	int hasEmissiveMap;

	vec3 reflectance;
	vec2 reflectanceTiling;
	sampler2D reflectanceMap;
	int hasReflectanceMap;

	float roughness;
	vec2 roughnessTiling;
	sampler2D roughnessMap;
	int hasRoughnessMap;

	float metallic;
	vec2 metallicTiling;
	sampler2D metallicMap;
	int hasMetallicMap;

	vec2 normalTiling;
	sampler2D normalMap;
	int hasNormalMap;
};

struct Enviornment {
	sampler2D skybox;
	vec3 irradiance;
};

struct Light {
	vec3 lightPosition;

	vec3 lightColor;

	int isDirectional;
};

uniform Material material;

uniform Enviornment enviornment;

uniform int lightCount;
uniform Light[10] lights;

// PBR Stuff
uniform vec3 cameraPosition;

layout(location = 0) out vec4 color;

void main() {
	vec3 albedo = (material.albedo * (1.0f - material.hasAlbedoMap)) + (material.albedo * texture2D(material.albedoMap, albedoCoordinate * material.albedoTiling).rgb * material.hasAlbedoMap);

	color = vec4(albedo, 1.0);
}