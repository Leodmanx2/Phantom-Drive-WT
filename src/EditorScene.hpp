#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "InputModel.hpp"
#include "Scene.hpp"
#include <SWI-cpp.h>

class EditorScene final : public Scene {
	private:
	InputModel m_inputModel;
	Camera     m_editorCamera;

	public:
	static EditorScene* activeScene;
	explicit EditorScene(const std::string& name);

	void update(std::chrono::milliseconds duration);
	void simulate(std::chrono::milliseconds duration);
	void draw();
	void processInput(GLFWwindow& window);

	void addActor(const std::string& name);
};

#endif
