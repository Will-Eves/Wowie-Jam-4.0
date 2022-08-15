#pragma once

namespace Burst {
	struct Button : ECS::Component {
		PanelRenderer* panel;
		TextRenderer* renderer;

		Color backgroundColor;
		Color backgroundColorClicked = Color(0.7f, 0.7f, 0.7f);
		float radius = 10.0f;
		Color textColor = Color(0.2f, 0.2f, 0.2f);
		std::string text = "Text";
		float fontSize = 50.0f;

		void(*OnClick)();
		bool hasCallback = false;

		void SetClickFunction(void(*callback)()) {
			OnClick = callback;
			hasCallback = true;
		}

		bool clicked = false;
		bool isDown = false;

		virtual void Start() {
			panel = this->parent->AddComponent<PanelRenderer>();
			panel->color = backgroundColor;
			panel->radius = radius;
			renderer = this->parent->AddComponent<TextRenderer>();
			renderer->color = textColor;
			renderer->text = text;
			renderer->fontSize = fontSize;
		}

		virtual void Render() {
			if (!clicked && Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
				Vector2 mouse = Input::GetMousePosition();

				if (mouse.x < UIManager::currentMax.x && mouse.y < UIManager::currentMax.y && mouse.x > UIManager::currentMin.x && mouse.y > UIManager::currentMin.y) {
					isDown = true;
					if(hasCallback) OnClick();
				}

				clicked = true;
			}
			if (Input::GetMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT)) {
				isDown = false;
				clicked = false;
			}

			panel->color = (isDown ? backgroundColorClicked : backgroundColor);
			panel->radius = radius;
			renderer->color = textColor;
			renderer->text = text;
			renderer->fontSize = fontSize;
		}
	};
}