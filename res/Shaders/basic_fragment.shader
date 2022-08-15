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
in vec3 fragmentPosition;
in vec3 normal;
uniform vec3 cameraPosition;
in mat3 normalMatrix;

vec3 fresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness){
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = 3.14159 * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness){
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v) {
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

vec4 PBR() {
	// AO = Ambient Occlusion

	vec3 emissive = (material.emissive * (1.0f - material.hasEmissiveMap)) + (texture2D(material.emissiveMap, albedoCoordinate * material.emissiveTiling).rgb * material.hasEmissiveMap);
	vec3 bloom = emissive;

	vec3 albedo = (material.albedo * (1.0f - material.hasAlbedoMap)) + (material.albedo * texture2D(material.albedoMap, albedoCoordinate * material.albedoTiling).rgb * material.hasAlbedoMap);

	// How much the light reflects at an angle of 0
	vec3 F0 = (material.reflectance * (1.0f - material.hasReflectanceMap)) + (texture2D(material.reflectanceMap, albedoCoordinate * material.reflectanceTiling).rgb * material.hasReflectanceMap);

	float metallic = (material.metallic * (1.0f - material.hasMetallicMap)) + (texture2D(material.metallicMap, albedoCoordinate * material.metallicTiling).r * material.hasMetallicMap);

	float roughness = (material.roughness * (1.0f - material.hasRoughnessMap)) + (texture2D(material.roughnessMap, albedoCoordinate * material.roughnessTiling).r * material.hasRoughnessMap);

	float ao = 1.0;

	vec3 N = (normalize(normal) * (1.0f - material.hasNormalMap)) + (normalize(normalMatrix * (texture2D(material.normalMap, albedoCoordinate * material.normalTiling).rgb * 2.0 - 1.0)) * material.hasNormalMap * -1.0f);

	vec3 V = normalize(cameraPosition - fragmentPosition);

	vec3 Lo = vec3(0.0);
	for (int i = 0; i < lightCount; i++) {
		vec3 L = normalize(lights[i].lightPosition - fragmentPosition * (1 - lights[i].isDirectional));
		vec3 H = normalize(V + L);

		float distance = length(lights[i].lightPosition - fragmentPosition);

		// Directional Light
		distance = length(lights[i].lightPosition - fragmentPosition);
		if (lights[i].isDirectional == 1) distance = 1.0f;
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lights[i].lightColor * attenuation;

		F0 = mix(F0, albedo, metallic);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		// Calculate IBR

		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
		vec3 specular = numerator / denominator;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;

		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / 3.14159 + specular) * radiance * NdotL;

		bloom += specular;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	// Get IBL
	vec3 reflectAngle = -1.0f * (V - 2.f * dot(V, N) * N);
	vec2 uv = SampleSphericalMap(normalize(reflectAngle));
	vec3 reflection = texture2D(enviornment.skybox, uv).rgb;

	uv = SampleSphericalMap(N);
	vec3 irradiance = texture2D(enviornment.skybox, uv).rgb;

	// Add IBL to current color
	// Do this later

	return vec4(color + emissive, 1.0);
}

layout(location = 0) out vec4 color;

void main() {
	color = PBR();
	color.w = material.alpha;

	if(color.w == 0) discard;
}