#include "EditorScene.hpp"

EditorScene* EditorScene::activeScene = nullptr;

// ---------------------------------------------------------------------------
//  Prolog Interface to Scene Editing Commands
// ---------------------------------------------------------------------------
// TODO: Move to appropriate modules

PREDICATE0(pd_save) { return false; }

PREDICATE(pd_saveas, 1) { return false; }

PREDICATE0(pd_exit) { return false; }

PREDICATE(pd_add_actor, 1) { return false; }

PREDICATE(pd_select, 2) { return false; }

PREDICATE0(pd_deselect) { return false; }

PREDICATE0(pd_remove_actor) { return false; }

PREDICATE(pd_move_by, 3) { return false; }

PREDICATE(pd_move_to, 3) { return false; }

PREDICATE(pd_rotate, 3) { return false; }

PREDICATE0(pd_add_light) { return false; }

PREDICATE(pd_select_light, 1) { return false; }

PREDICATE0(pd_remove_light) { return false; }

PREDICATE0(pd_edit_light) { return false; }

PREDICATE0(pd_load_assets) { return false; }

// ---------------------------------------------------------------------------
//  Scene Overrides
// ---------------------------------------------------------------------------

// TODO: Implement simple dev console

EditorScene::EditorScene(const std::string& name)
  : Scene(name), m_inputModel("SceneEdit") {
	//TODO: Editor mode scene loading
	// The scene will load normally first.
	// If the scene file can't be found, we create a new one.
	// If the scene file is corrupt, we give up.
}

// The editor works with the initial moment of a scene.
// We don't want these functions to do anything.
void EditorScene::update(std::chrono::milliseconds) {}
void EditorScene::simulate(std::chrono::milliseconds) {}

// Similarly, we don't want the Actors processing any input.
// Input processing is limited to editor and camera input.
void EditorScene::processInput(GLFWwindow& window) {
	m_inputModel.update(window);
	m_editorCamera.processInput(window);
}

// The scene should draw as per usual, though.
void EditorScene::draw() { Scene::draw(); }

// ---------------------------------------------------------------------------
//  Editor Functionality
// ---------------------------------------------------------------------------

void EditorScene::addActor(const std::string& name) {
	try {
		m_actors.emplace(
		  std::make_pair(++m_highestID, std::make_unique<Actor>(name)));
	} catch(const std::exception& exception) {
		g_logger->write(Logger::LOG_ERROR, exception.what());
		throw std::runtime_error("Failed to load Actor");
	}
}
