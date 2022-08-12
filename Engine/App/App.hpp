#pragma once

namespace Burst {
	namespace App {
		bool running = true;

		void Quit() {
			running = false;
		}

		void PollEvents() {
			glfwPollEvents();
		}

		void Terminate() {
			glfwTerminate();
		}
	}
}