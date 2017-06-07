#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "InputModel.hpp"
#include "Scene.hpp"
#include <PSCN.hpp>
#include <SWI-cpp.h>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>

class EditorScene final : public Scene {
	private:
	InputModel m_inputModel;
	Camera     m_editorCamera;
	Shader*    m_defaultShader;

	std::queue<std::string> m_commands;
	std::mutex              m_mutex;

	int m_selected;

	bool        m_runConsole;
	std::thread m_consoleThread;

	public:
	const std::string   name;
	static EditorScene* activeScene;
	explicit EditorScene(const std::string& name);
	~EditorScene();

	void update(std::chrono::milliseconds duration);
	void simulate(std::chrono::milliseconds duration);
	void draw();
	void processInput(GLFWwindow& window);

	void addActor(const std::string& name);
	void removeActor(int id);
	void setSelected(int id);
	void saveAs(const std::string& file);
	int    getSelectedID();
	Actor* getSelectedActor();
};

#endif
