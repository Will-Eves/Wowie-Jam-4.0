#pragma once

namespace Burst {
	struct Transform {
		Vector3 position;
		Vector3 rotation;
		Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);

		glm::mat4 GetModelMatrix() {
			glm::mat4 modelMatrix(1.0);
			modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, -position.z));
			modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, -rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, -rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(scale.x, scale.y, scale.z));
			return modelMatrix;
		}

		glm::mat4 GetViewMatrix() {
			glm::mat4 viewMatrix(1.0);
			viewMatrix = glm::rotate(viewMatrix, -rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			viewMatrix = glm::rotate(viewMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			viewMatrix = glm::rotate(viewMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			viewMatrix = glm::translate(viewMatrix, glm::vec3(-position.x, -position.y, position.z));
			return viewMatrix;
		}
	};
}