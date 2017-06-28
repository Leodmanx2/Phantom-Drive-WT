#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "InputModel.hpp"
#include "Scene.hpp"
#include <PSCN.hpp>
#include <SWI-cpp.h>
#include <cstdlib>
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

	void update(const std::chrono::milliseconds& duration) override;
	void simulate(const std::chrono::milliseconds& duration) override;
	void draw() override;
	void process(std::queue<KeyEvent>& keyEvents) override;
	void process(std::queue<MouseButtonEvent>& buttonEvents) override;
	void process(std::queue<MouseMovementEvent>& movementEvents) override;

	void addActor(const std::string& actorName);
	constexpr void removeActor(int id) { m_actors.erase(id); }
	constexpr void setSelected(int id) { m_selected = id; }
	void saveAs(const std::string& file);
	constexpr int selectedID() { return m_selected; }
	Actor*        selectedActor();
};

#endif
