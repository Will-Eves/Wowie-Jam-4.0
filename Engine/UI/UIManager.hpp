#pragma once

namespace Burst {
	namespace UIManager {
		glm::mat4 projectionMatrix;

		Mesh panelMesh({ 0.0f }, { 0 });

		Shader uiShader;

		AnchorPoint currentAnchor = Anchors::TopLeft;
		Vector2 currentMin;
		Vector2 currentMax;

		void UpdateScreenSize(GLFWwindow* window, int width, int height) {
			// recreate projection matrix
			projectionMatrix = glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height, 0.01f, 100.0f);

			Anchors::TopLeft.point = Vector2(0.0f, Window::height);
			Anchors::TopMiddle.point = Vector2(Window::width / 2.0f, Window::height);
			Anchors::TopRight.point = Vector2(Window::width, Window::height);

			Anchors::MiddleLeft.point = Vector2(0.0f, Window::height / 2.0f);
			Anchors::MiddleMiddle.point = Vector2(Window::width / 2.0f, Window::height / 2.0f);
			Anchors::MiddleRight.point = Vector2(Window::width, Window::height / 2.0f);

			Anchors::BottomLeft.point = Vector2(0.0f, 0.0f);
			Anchors::BottomMiddle.point = Vector2(Window::width / 2.0f, 0.0f);
			Anchors::BottomRight.point = Vector2(Window::width, 0.0f);
		}

		void Setup() {
			FontManager::Setup();

			uiShader = Shader("engine_res/Shaders/ui_vertex.shader", "engine_res/Shaders/ui_fragment.shader", true);

			std::vector<float> verts = {
				-0.5f, -0.5f, 0.0f, 0.0f,
				-0.5f,  0.5f, 0.0f, 1.0f,
				 0.5f,  0.5f, 1.0f, 1.0f,
				 0.5f, -0.5f, 1.0f, 0.0f
			};

			std::vector<unsigned int> inds = {
				0, 2, 1,
				0, 3, 2
			};

			panelMesh = Mesh(verts, inds);

			// create projection matrix
			projectionMatrix = glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height, 0.01f, 100.0f);

			Anchors::TopLeft.point = Vector2(0.0f, Window::height);
			Anchors::TopMiddle.point = Vector2(Window::width / 2.0f, Window::height);
			Anchors::TopRight.point = Vector2(Window::width, Window::height);

			Anchors::MiddleLeft.point = Vector2(0.0f, Window::height / 2.0f);
			Anchors::MiddleMiddle.point = Vector2(Window::width / 2.0f, Window::height / 2.0f);
			Anchors::MiddleRight.point = Vector2(Window::width, Window::height / 2.0f);

			Anchors::BottomLeft.point = Vector2(0.0f, 0.0f);
			Anchors::BottomMiddle.point = Vector2(Window::width / 2.0f, 0.0f);
			Anchors::BottomRight.point = Vector2(Window::width, 0.0f);

			Window::AddFramebufferSizeCallback(UpdateScreenSize);
		}

		void Update() {
			uiShader.Bind();
			uiShader.SendMatrix4("projectionMatrix", projectionMatrix);
		}
	}
}