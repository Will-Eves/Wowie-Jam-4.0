#pragma once

namespace Burst {
	struct Material {
		Color albedo;
		Texture* albedoMap = nullptr;
		Vector2 albedoTiling = Vector2(1.0f, 1.0f);

		Color emissive = Color(0.0f, 0.0f, 0.0f);
		Texture* emissiveMap = nullptr;
		Vector2 emissiveTiling = Vector2(1.0f, 1.0f);

		Color reflectance = Color(0.04f, 0.04f, 0.04f);
		Texture* reflectanceMap = nullptr;
		Vector2 reflectanceTiling = Vector2(1.0f, 1.0f);

		float roughness = 0.75f;
		Texture* roughnessMap = nullptr;
		Vector2 roughnessTiling = Vector2(1.0f, 1.0f);

		float metallic = 0.05f;
		Texture* metallicMap = nullptr;
		Vector2 metallicTiling = Vector2(1.0f, 1.0f);

		Texture* normalMap = nullptr;
		Vector2 normalTiling = Vector2(1.0f, 1.0f);

		Shader* shader;

		Material(Shader* _shader) {
			shader = _shader;
		}

		void Bind(bool ovveride=false) {
			shader->SendFloat1("material.alpha", albedo.a);

			shader->SendFloat3("material.albedo", albedo.r, albedo.g, albedo.b);
			shader->SendFloat2("material.albedoTiling", albedoTiling.x, albedoTiling.y);
			shader->SendInt1("material.albedoMap", 0);
			if (albedoMap != nullptr) albedoMap->Bind(0);
			shader->SendInt1("material.hasAlbedoMap", albedoMap != nullptr ? 1 : 0);

			shader->SendFloat3("material.emissive", emissive.r, emissive.g, emissive.b);
			shader->SendFloat2("material.emissiveTiling", emissiveTiling.x, emissiveTiling.y);
			shader->SendInt1("material.emissiveMap", 1);
			if (emissiveMap != nullptr) emissiveMap->Bind(1);
			shader->SendInt1("material.hasEmissiveMap", emissiveMap != nullptr ? 1 : 0);

			shader->SendFloat3("material.reflectance", reflectance.r, reflectance.g, reflectance.b);
			shader->SendFloat2("material.reflectanceTiling", reflectanceTiling.x, reflectanceTiling.y);
			shader->SendInt1("material.reflectanceMap", 2);
			if (reflectanceMap != nullptr) reflectanceMap->Bind(2);
			shader->SendInt1("material.hasReflectanceMap", reflectanceMap != nullptr ? 1 : 0);

			shader->SendFloat1("material.roughness", roughness);
			shader->SendFloat2("material.roughnessTiling", roughnessTiling.x, roughnessTiling.y);
			shader->SendInt1("material.roughnessMap", 3);
			if (roughnessMap != nullptr) roughnessMap->Bind(3);
			shader->SendInt1("material.hasRoughnessMap", roughnessMap != nullptr ? 1 : 0);

			shader->SendFloat1("material.metallic", metallic);
			shader->SendFloat2("material.metallicTiling", metallicTiling.x, metallicTiling.y);
			shader->SendInt1("material.metallicMap", 4);
			if (metallicMap != nullptr) metallicMap->Bind(4);
			shader->SendInt1("material.hasMetallicMap", metallicMap != nullptr ? 1 : 0);

			shader->SendFloat2("material.normalTiling", normalTiling.x, normalTiling.y);
			shader->SendInt1("material.normalMap", 5);
			if (normalMap != nullptr) normalMap->Bind(5);
			shader->SendInt1("material.hasNormalMap", normalMap != nullptr ? 1 : 0);
		}
	};
}