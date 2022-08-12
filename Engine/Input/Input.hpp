#pragma once

namespace Burst {
	namespace Input {
		std::map<int, bool> keys;
		std::map<int, bool> mouseButtons;
		Vector2 mousePosition;

		void setKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS) {
				keys[key] = true;
			}
			else if (action == GLFW_RELEASE) {
				keys[key] = false;
			}
		}

		void setCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
			mousePosition = Vector2(xpos, ypos);
		}

		void setMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
			if (action == GLFW_PRESS) {
				mouseButtons[button] = true;
			}
			else if (action == GLFW_RELEASE) {
				mouseButtons[button] = false;
			}
		}

		void setScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
			// xoffset is useless
			// yoffset is scroll
		}

		Vector2 GetMousePosition() {
			return mousePosition;
		}

		void ShowCursor() {
			glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		void HideCursor() {
			glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		void EnableCursor() {
			glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		void DisableCursor() {
			glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		bool GetKeyDown(int key) {
			return keys[key];
		}

		bool GetKeyUp(int key) {
			return !keys[key];
		}

		bool GetCharDown(char key) {
			key = toupper(key);
			return keys[key];
		}

		bool GetCharUp(char key) {
			key = toupper(key);
			return !keys[key];
		}

		bool GetMouseButtonDown(int button) {
			return mouseButtons[button];
		}

		bool GetMouseButtonUp(int button) {
			return !mouseButtons[button];
		}

		double GetHorizontal() {
			double out = 0.0;

			if (GetKeyDown(GLFW_KEY_A) || GetKeyDown(GLFW_KEY_LEFT)) {
				out -= 1.0;
			}

			if (GetKeyDown(GLFW_KEY_D) || GetKeyDown(GLFW_KEY_RIGHT)) {
				out += 1.0;
			}

			return out;
		}

		double GetVertical() {
			double out = 0.0;

			if (GetKeyDown(GLFW_KEY_S) || GetKeyDown(GLFW_KEY_DOWN)) {
				out -= 1.0;
			}

			if (GetKeyDown(GLFW_KEY_W) || GetKeyDown(GLFW_KEY_UP)) {
				out += 1.0;
			}

			return out;
		}

		bool GetControllerActive(int controller) {
			return glfwJoystickPresent(GLFW_JOYSTICK_1 + controller);
		}

		std::vector<Vector2> GetControllerAxes(int controller) {
			std::vector<Vector2> out;
			int count;
			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + controller, &count);
			for (int i = 0; i < count; i += 2) {
				out.push_back(Vector2(axes[i], axes[i + 1]));
			}
			return out;
		}

		std::vector<bool> GetControllerButtons(int controller) {
			std::vector<bool> out;
			int count;
			const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1 + controller, &count);
			for (int i = 0; i < count; i++) {
				if (buttons[i] == GLFW_PRESS) out.push_back(true);
				if (buttons[i] == GLFW_RELEASE) out.push_back(false);
			}
			return out;
		}

		std::vector<Vector2> GetControllerHats(int controller) {
			std::vector<Vector2> out;
			int count;
			const unsigned char* hats = glfwGetJoystickHats(GLFW_JOYSTICK_1 + controller, &count);
			for (int i = 0; i < count; i++) {
				Vector2 o;
				if (hats[i] & GLFW_HAT_UP) {
					o.y += 1.0f;
				}
				if (hats[i] & GLFW_HAT_DOWN) {
					o.y -= 1.0f;
				}
				if (hats[i] & GLFW_HAT_LEFT) {
					o.x -= 1.0f;
				}
				if (hats[i] & GLFW_HAT_RIGHT) {
					o.x += 1.0f;
				}
				out.push_back(o);
			}
			return out;
		}

		std::string GetControllerName(int controller) {
			const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1 + controller);
			return name;
		}

		struct Controller {
			unsigned int index = 0;
			bool active = false;
			std::vector<Vector2> axes;
			std::vector<bool> buttons;
			std::vector<Vector2> hats;
			std::string name = "None";

			Controller(unsigned int _index=0) {
				this->index = _index;
			}

			void Update() {
				this->active = GetControllerActive(this->index);

				if (!this->active) return;

				this->axes = GetControllerAxes(this->index);
				this->buttons = GetControllerButtons(this->index);
				this->hats = GetControllerHats(this->index);
				this->name = GetControllerName(this->index);
			}
		};

#define MAX_CONTROLLERS 16

		Controller controllers[MAX_CONTROLLERS];

		Controller* GetController(unsigned int index = 0) {
			return &controllers[index];
		}

		void setJoystickCallback(int jid, int evnt) {
			if (evnt == GLFW_CONNECTED) {
				controllers[jid].active = true;
			}
			else if (evnt == GLFW_DISCONNECTED) {
				controllers[jid].active = false;
			}
		}

		void Setup() {
			// Set all callbacks
			Window::AddKeyCallback(setKeyCallback);

			Window::AddCursorPosCallback(setCursorPosCallback);

			Window::AddMouseButtonCallback(setMouseButtonCallback);

			Window::AddScrollCallback(setScrollCallback);

			Window::AddJoystickCallback(setJoystickCallback);

			// Create all controller objects
			for (int i = 0; i < MAX_CONTROLLERS; i++) {
				controllers[i] = Controller(i);
			}
		}

		void Update() {
			// Update all controllers
			for (int i = 0; i < MAX_CONTROLLERS; i++) {
				controllers[i].Update();
			}
		}
	}
}