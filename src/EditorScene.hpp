#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "InputModel.hpp"
#include "Scene.hpp"
#include <SWI-cpp.h>
#include <mutex>
#include <queue>
#include <thread>

class EditorScene final : public Scene {
	private:
	InputModel m_inputModel;
	Camera     m_editorCamera;

	std::queue<std::string> m_commands;
	std::mutex              m_mutex;

	bool        m_runConsole;
	std::thread m_consoleThread;

	public:
	static EditorScene* activeScene;
	explicit EditorScene(const std::string& name);
	~EditorScene();

	void update(std::chrono::milliseconds duration);
	void simulate(std::chrono::milliseconds duration);
	void draw();
	void processInput(GLFWwindow& window);

	void addActor(const std::string& name);
};

#endif
