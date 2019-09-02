#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "PSCN.hpp"
#include "Scene.hpp"
#include <SWI-cpp.h>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>

class EditorScene final : public Scene {
	private:
	Camera  m_editorCamera;
	Shader* m_defaultShader;

	std::queue<std::string> m_commands;
	std::mutex              m_mutex;

	int m_selected;

	public:
	const std::string name;
	explicit EditorScene(const std::string& name);
	~EditorScene();

	void update(const std::chrono::milliseconds& duration) override;
	void simulate(const std::chrono::milliseconds& duration) override;
	void draw() override;
	void process(std::queue<KeyEvent>& keyEvents) override;
	void process(std::queue<MouseButtonEvent>& buttonEvents) override;
	void process(std::queue<MouseMovementEvent>& movementEvents) override;

	void   addActor(const std::string& actorName);
	void   removeActor(int id);
	void   setSelected(int id);
	void   saveAs(const std::string& file);
	int    selectedID();
	Actor* selectedActor();
};

#endif
