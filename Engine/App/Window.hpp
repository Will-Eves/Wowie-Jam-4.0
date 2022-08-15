#pragma once

#define CLEAR_GL_ERROR() { GLenum err;while ((err = glGetError()) != GL_NO_ERROR){ continue; } }

std::map<GLenum, std::string> PRINT_GL_ERROR_CODES = { { 0, "No Error" }, { 1280, "Enumaration parameter is not legal" }, { 1281, "Value parameter is not legal" }, { 1282, "Operation is not legal for given parameters" }, { 1283, "Stack Overflow" }, { 1284, "Stack Underflow" }, { 1285, "Out of memory" }, { 1286, "Framebuffer is not complete" } };
#define PRINT_GL_ERROR() { GLenum err;while ((err = glGetError()) != GL_NO_ERROR){ std::cout << "OpenGL Error : " << PRINT_GL_ERROR_CODES[err] << std::endl; } }

#define PRINT_GL_ERROR_LINE(x) CLEAR_GL_ERROR(); x; PRINT_GL_ERROR();

namespace Burst {
	namespace Window {
        GLFWwindow* window;

        int width;
        int height;

        std::vector<void(*)(GLFWwindow*, int, int)> setFramebufferSizeCallbacks;
        std::vector<void(*)(GLFWwindow*, int, int, int, int)> setKeyCallbacks;
        std::vector<void(*)(GLFWwindow*, double, double)> setCursorPosCallbacks;
        std::vector<void(*)(GLFWwindow*, int, int, int)> setMouseButtonCallbacks;
        std::vector<void(*)(GLFWwindow*, double, double)> setScrollCallbacks;
        std::vector<void(*)(int, int)> setJoystickCallbacks;

        void AddFramebufferSizeCallback(void(*callback)(GLFWwindow*, int, int)) {
            setFramebufferSizeCallbacks.push_back(callback);
        }

        void SetFramebufferSizeCallback(GLFWwindow* _window, int _width, int _height) {
            width = _width;
            height = _height;
            for (void(*callback)(GLFWwindow*, int, int) : setFramebufferSizeCallbacks) {
                callback(_window, _width, _height);
            }
        }

        void AddKeyCallback(void(*callback)(GLFWwindow*, int, int, int, int)) {
            setKeyCallbacks.push_back(callback);
        }

        void SetKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods) {
            for (void(*callback)(GLFWwindow*, int, int, int, int) : setKeyCallbacks) {
                callback(_window, _key, _scancode, _action, _mods);
            }
        }

        void AddCursorPosCallback(void(*callback)(GLFWwindow*, double, double)) {
            setCursorPosCallbacks.push_back(callback);
        }

        void SetCursorPosCallback(GLFWwindow* _window, double _xpos, double _ypos) {
            for (void(*callback)(GLFWwindow*, double, double) : setCursorPosCallbacks) {
                callback(_window, _xpos, _ypos);
            }
        }

        void AddMouseButtonCallback(void(*callback)(GLFWwindow*, int, int, int)) {
            setMouseButtonCallbacks.push_back(callback);
        }

        void SetMouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods) {
            for (void(*callback)(GLFWwindow*, int, int, int) : setMouseButtonCallbacks) {
                callback(_window, _button, _action, _mods);
            }
        }

        void AddScrollCallback(void(*callback)(GLFWwindow*, double, double)) {
            setScrollCallbacks.push_back(callback);
        }

        void SetScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset) {
            for (void(*callback)(GLFWwindow*, double, double) : setScrollCallbacks) {
                callback(_window, _xoffset, _yoffset);
            }
        }

        void AddJoystickCallback(void(*callback)(int, int)) {
            setJoystickCallbacks.push_back(callback);
        }

        void SetJoystickCallback(int jid, int evnt) {
            for (void(*callback)(int, int) : setJoystickCallbacks) {
                callback(jid, evnt);
            }
        }

        bool Setup(int _width=800, int _height=800, std::string name="Burst Engine : Game", bool canResize=false, bool fullscreen=false) {
            if (!glfwInit()) return false;

            width = _width;
            height = _height;

            if (fullscreen) {
                const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

                width = mode->width;
                height = mode->height;
            }

            window = glfwCreateWindow(width, height, name.c_str(), (fullscreen ? glfwGetPrimaryMonitor() : NULL), NULL);
            if (!window) {
                glfwTerminate();
                return false;
            }

            glfwMakeContextCurrent(window);

            GLenum err = glewInit();
            if (GLEW_OK != err) {
                return false;
            }

            // Sets GLFW Version
            glfwWindowHint(GLFW_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_VERSION_MINOR, 4);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwSetWindowAttrib(window, GLFW_RESIZABLE, canResize);

            // Turns on vsync
            glfwSwapInterval(0);

            // Enabling basic OpenGL Tests
            // Depth Testing
            glEnable(GL_DEPTH_TEST);

            // Alpha Blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Back face culling
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            glFrontFace(GL_CW);

            // Input Callbacks

            glfwSetFramebufferSizeCallback(window, SetFramebufferSizeCallback);
            glfwSetKeyCallback(window, SetKeyCallback);
            glfwSetCursorPosCallback(window, SetCursorPosCallback);
            glfwSetMouseButtonCallback(window, SetMouseButtonCallback);
            glfwSetScrollCallback(window, SetScrollCallback);
            glfwSetJoystickCallback(SetJoystickCallback);

            return true;
        }

        void SwapBuffers() {
            glfwSwapBuffers(window);
        }

        bool WindowShouldClose() {
            return glfwWindowShouldClose(window);
        }

        GLFWwindow* GetWindow() {
            return window;
        }
	}
}