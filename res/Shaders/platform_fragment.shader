#version 410 core

in vec2 aCord;

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
in vec3 fragmentPosition;
in vec3 normal;
uniform vec3 cameraPosition;

vec4 PBR() {
	vec3 NT = normalize(normal);

	vec2 albedoCoordinate;
	if (NT.z != 0.0f) {
		albedoCoordinate = vec2(fragmentPosition.x, fragmentPosition.y);
	}
	else if (NT.x != 0.0f) {
		albedoCoordinate = vec2(fragmentPosition.z, fragmentPosition.y);
	}
	else if (NT.y != 0.0f) {
		albedoCoordinate = vec2(fragmentPosition.x, fragmentPosition.z);
	}

	vec3 lavaAlbedo = texture2D(material.emissiveMap, albedoCoordinate * material.emissiveTiling / 2.0f).rgb;
	vec3 lavaEmissive = texture2D(material.reflectanceMap, albedoCoordinate * material.reflectanceTiling / 2.0f).rgb;

	vec3 albedo = texture2D(material.albedoMap, albedoCoordinate * material.albedoTiling / 2.0f).rgb;

	float D = dot(NT, lights[0].lightPosition);
	D = max(D, 0.0);
	vec3 color = vec3(0.07) * albedo + albedo * D;

	float amt = fragmentPosition.y + 5.0f;
	if (amt < 0.0f) amt = 0.0f;
	else if (amt > 1.0f) amt = 1.0f;
	color = (color * amt) + (vec3(0.07) * lavaAlbedo * (1.0f - amt));

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	return vec4(color + ((1.0 - amt) * lavaEmissive), 1.0);
}

layout(location = 0) out vec4 color;

void main() {
	color = PBR();
	color.w = material.alpha;

	if(color.w == 0) discard;
}