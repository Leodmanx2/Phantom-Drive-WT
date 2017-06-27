#ifndef APPLICATION_H
#define APPLICATION_H

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#define GLFW_INCLUDE_NONE

#include "EditorScene.hpp"
#include "Logger.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "input.hpp"
#include <GLFW/glfw3.h>
#include <SWI-cpp.h>
#include <boost/functional/hash.hpp> // TODO: Do not pull in Boost!
#include <functional>
#include <iostream>
#include <physfs.h>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

class Application final {
	private:
	std::shared_ptr<Window> m_window;
	Renderer                m_renderer;

	static std::mutex s_keyQueueMutex;
	static std::unordered_set<std::pair<int, int>,
	                          boost::hash<std::pair<int, int>>>
	                            s_keysPressed;
	static std::queue<KeyEvent> s_keyQueue;

	static std::mutex                   s_buttonQueueMutex;
	static std::queue<MouseButtonEvent> s_buttonQueue;

	static std::mutex                     s_movementQueueMutex;
	static std::queue<MouseMovementEvent> s_movementQueue;

	void initFilesystem(int argc, char** argv);

	static void
	key_callback(GLFWwindow*, int key, int, int action, int modifiers);
	static void
	mouse_button_callback(GLFWwindow*, int button, int action, int mods);
	static void
	cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	void processInput();
	void draw();

	Scene* m_scene; // TODO: Temporary variable for dev. purposes. Remove.

	public:
	Application(int argc, char** argv);
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	~Application();

	void run();
};

#endif
