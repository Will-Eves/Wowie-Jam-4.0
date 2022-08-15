#pragma once

namespace Burst {
	namespace LightManager {
		unsigned int lightCount = 0;
		unsigned int maxLights = 10;
	}

	struct Light : ECS::Component {
		Color color = Color(1.0f, 0.95f, 0.95f);
		float intensity = 1.0f;

		bool isDirectional = false;

		unsigned int lightIndex;

		Light() {
			this->lightIndex = LightManager::lightCount;
			LightManager::lightCount++;

			for (Shader* shader : ShaderManager::shaders) {
				shader->SendInt1("lightCount", LightManager::lightCount);
			}
		}

		virtual void Update() {
			for (Shader* shader : ShaderManager::shaders) {
				shader->SendFloat3("lights[" + std::to_string(this->lightIndex) + "].lightPosition", this->parent->transform.position.x, this->parent->transform.position.y, -this->parent->transform.position.z);
				shader->SendFloat3("lights[" + std::to_string(this->lightIndex) + "].lightColor", this->color.r * this->intensity, this->color.g * this->intensity, this->color.b * this->intensity);
				shader->SendInt1("lights[" + std::to_string(this->lightIndex) + "].isDirectional", this->isDirectional);
			}
		}
	};
}