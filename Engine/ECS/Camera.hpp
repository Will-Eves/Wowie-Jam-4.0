namespace Burst {
	struct Camera : public ECS::Component {
		glm::mat4 projectionMatrix;

		Camera(float vertical_fov=45.0f, float near_clip_plane=0.01f, float far_clip_plane=1000.0f) {
			this->projectionMatrix = glm::perspective(vertical_fov, (float)Window::width / (float)Window::height, near_clip_plane, far_clip_plane);
		}

		virtual void Start() {
			Burst::Enviornment::SetProjectionMatrix(this->projectionMatrix);

			for (Shader* shader : ShaderManager::shaders) {
				shader->SendMatrix4("projectionMatrix", this->projectionMatrix);
			}
		}

		virtual void Update() {
			glm::mat4 viewMatrix = this->parent->transform.GetViewMatrix();

			Burst::Enviornment::SetCameraMatrix(viewMatrix);
		}

		virtual void Render() {
			glm::mat4 viewMatrix = this->parent->transform.GetViewMatrix();

			float x = this->parent->transform.position.x;
			float y = this->parent->transform.position.y;
			float z = -this->parent->transform.position.z;

			for (Shader* shader : ShaderManager::shaders) {
				shader->SendFloat3("cameraPosition", x, y, z);
				shader->SendMatrix4("viewMatrix", viewMatrix);
			}
		}
	};
}