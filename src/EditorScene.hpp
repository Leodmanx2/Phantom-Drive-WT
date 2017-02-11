#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "InputModel.hpp"
#include "Scene.hpp"

class EditorScene final : public Scene {
	private:
	InputModel m_inputModel;

	public:
	explicit EditorScene(const std::string& name);

	void update(std::chrono::milliseconds duration);
	void simulate(std::chrono::milliseconds duration);
	void draw();
	void processInput(GLFWwindow& window);
};

#endif
