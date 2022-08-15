#pragma once

namespace Burst {
	namespace Engine {
		std::vector<ECS::Scene*> scenes;
		ECS::Scene* currentScene = nullptr;

		std::vector<void(*)()> systemStarts;
		std::vector<void(*)()> systemUpdates;

		void AddSystemStart(void(*callback)()) {
			systemStarts.push_back(callback);
		}

		void AddSystemUpdate(void(*callback)()) {
			systemUpdates.push_back(callback);
		}

		bool Setup(int windowWidth=800, int windowHeight=450, std::string windowName="Game", bool resize=false, bool fullscreen=false) {
			if (!Window::Setup(windowWidth, windowHeight, "Burst Engine : " + windowName, resize, fullscreen)) {
				std::cout << "Burst Engine could not run because of an OpenGL error." << std::endl;
				return false;
			}

			Input::Setup();

			Enviornment::Setup();

			UIManager::Setup();

			return true;
		}

		bool AddScene(ECS::Scene* scene) {
			scenes.push_back(scene);

			return true;
		}

		bool LoadScene(unsigned int index) {
			if (index > scenes.size() - 1) return false;

			currentScene = scenes[index];

			currentScene->Start();

			return true;
		}

		bool Run() {
			if (scenes.size() == 0) {
				std::cout << "Burst Engine could not run because no scenes were added." << std::endl;
				return false;
			}

			currentScene = scenes[0];

			for (void(*func)() : systemStarts) {
				func();
			}

			currentScene->Start();

			Time::Setup();

			while (!Window::WindowShouldClose() && App::running) {
				for (void(*func)() : systemUpdates) {
					func();
				}

				Time::Update();

				Input::Update();

				Renderer::Clear();

				// Update Scene
				currentScene->Update();

				// Render Enviornment
				Enviornment::Render();
				Enviornment::SendSkyboxToShaders();

				// Render Scene
				currentScene->Render();

				glDisable(GL_DEPTH_TEST);

				UIManager::Update();

				currentScene->UIRender();

				glEnable(GL_DEPTH_TEST);

				Window::SwapBuffers();

				App::PollEvents();
			}

			Audio::StopAll();
			App::Terminate();

			return true;
		}
	}
}