#include "EditorScene.hpp"

EditorScene* EditorScene::activeScene = nullptr;

// ---------------------------------------------------------------------------
//  Prolog Interface to Scene Editing Commands
// ---------------------------------------------------------------------------
// TODO: Rename to indicate editor-specificity

PREDICATE0(pd_save) { return false; }

PREDICATE(pd_saveas, 1) { return false; }

PREDICATE0(pd_exit) { return false; }

PREDICATE(pd_add_actor, 1) {
	try {
		EditorScene::activeScene->addActor(static_cast<const char*>(A1));
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LOG_ERROR, exception.what());
		return false;
	}
	return true;
}

PREDICATE0(pd_select) {
	int selected =
	  g_renderer.pick(g_renderer.width() / 2, g_renderer.height() / 2);
	std::cout << selected << "\n";
	EditorScene::activeScene->setSelected(selected);
	return true;
}

PREDICATE(pd_select, 3) {
	int x = static_cast<int>(A1);
	int y = static_cast<int>(A2);
	A3    = g_renderer.pick(x, y);
	return true;
}

PREDICATE0(pd_deselect) {
	EditorScene::activeScene->setSelected(0);
	return true;
}

PREDICATE0(pd_remove_actor) {
	int selected = EditorScene::activeScene->getSelectedID();
	EditorScene::activeScene->removeActor(selected);
	return true;
}

PREDICATE(pd_move_by, 3) {
	double longitude = static_cast<double>(A1);
	double latitude  = static_cast<double>(A2);
	double altitude  = static_cast<double>(A3);
	auto   actor     = EditorScene::activeScene->getSelectedActor();
	if(!actor) return false;
	actor->translate(longitude, latitude, altitude);
	return true;
}

PREDICATE(pd_move_to, 3) {
	double x     = static_cast<double>(A1);
	double y     = static_cast<double>(A2);
	double z     = static_cast<double>(A3);
	auto   actor = EditorScene::activeScene->getSelectedActor();
	if(!actor) return false;
	actor->setPosition(x, y, z);
	return true;
}

PREDICATE(pd_rotate_by, 3) {
	double roll  = static_cast<double>(A1);
	double pitch = static_cast<double>(A2);
	double yaw   = static_cast<double>(A3);
	auto   actor = EditorScene::activeScene->getSelectedActor();
	if(!actor) return false;
	actor->rotate(roll, pitch, yaw);
	return true;
}

PREDICATE(pd_rotate_to, 3) {
	double roll  = glm::radians(static_cast<double>(A1));
	double pitch = glm::radians(static_cast<double>(A2));
	double yaw   = glm::radians(static_cast<double>(A3));
	auto   actor = EditorScene::activeScene->getSelectedActor();
	if(!actor) return false;
	glm::quat quat = glm::quat(glm::vec3(pitch, yaw, roll));
	actor->setOrientation(quat.w, quat.x, quat.y, quat.z);
	return true;
}

PREDICATE0(pd_add_light) { return false; }

PREDICATE(pd_select_light, 1) { return false; }

PREDICATE0(pd_remove_light) { return false; }

PREDICATE0(pd_edit_light) { return false; }

PREDICATE0(pd_load_assets) { return false; }

// ---------------------------------------------------------------------------
//  Scene Overrides
// ---------------------------------------------------------------------------

EditorScene::EditorScene(const std::string& name)
  : Scene(name)
  , m_inputModel("SceneEdit")
  , m_mutex()
  , m_selected(0)
  , m_runConsole(true)
  , m_consoleThread([&]() {
	  while(m_runConsole) {
		  std::string command;
		  std::getline(std::cin, command);
		  if(command.at(command.size() - 1) == '\r')
			  command.resize(command.size() - 1);
		  m_mutex.lock();
		  m_commands.emplace(command);
		  m_mutex.unlock();
	  }
	}) {
	// TODO: Editor mode scene loading
	// The scene will load normally first.
	// If the scene file can't be found, we create a new one.
	// If the scene file is corrupt, we give up.
	try {
		m_defaultShader = new Shader("textured.vert.glsl", "textured.frag.glsl");
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LOG_ERROR, exception.what());
	}

	m_ambience = 0.2f;
}

EditorScene::~EditorScene() {
	m_runConsole = false;
	std::cout << "Press ENTER to quit.\n";
	m_consoleThread.join();
	delete m_defaultShader;
}

// The editor works with the initial moment of a scene.
// We don't want these functions to do anything.
void EditorScene::update(std::chrono::milliseconds) {}
void EditorScene::simulate(std::chrono::milliseconds) {}

// Similarly, we don't want the Actors processing any input.
// Input processing is limited to editor and camera input.
void EditorScene::processInput(GLFWwindow& window) {
	//m_inputModel.update(window);
	m_editorCamera.processInput(window);

	// Process command queue
	EditorScene::activeScene = this;
	std::lock_guard<std::mutex> lock(m_mutex);
	while(!m_commands.empty()) {
		std::string command = m_commands.front();
		std::cout << (PlCall("do_command", {{command.c_str()}}) ?
		                "Command Succeeded" :
		                "Command Failed")
		          << "\n";
		m_commands.pop();
	}
	EditorScene::activeScene = nullptr;
}

// The scene should draw as per usual, though.
//void EditorScene::draw() { Scene::draw(); }

void EditorScene::draw() {
	glm::mat4 projectionMatrix =
	  glm::perspective(45.0f,
	                   static_cast<float>(g_renderer.width()) /
	                     static_cast<float>(g_renderer.height()),
	                   0.1f,
	                   10000.0f);
	for(auto actor = m_actors.begin(); actor != m_actors.end(); ++actor) {
		m_defaultShader->setAmbience(m_ambience);

		// Send camera position to the GPU.
		m_defaultShader->setViewMatrix(m_editorCamera.getViewMatrix());
		m_defaultShader->setEyePosition(glm::vec3(m_editorCamera.getPosition()));

		m_defaultShader->setProjectionMatrix(projectionMatrix);
		m_defaultShader->setObjectID(actor->first);
		actor->second->draw(*m_defaultShader);
	}
}

// ---------------------------------------------------------------------------
//  Editor Functionality
// ---------------------------------------------------------------------------

void EditorScene::addActor(const std::string& name) {
	try {
		m_actors.emplace(
		  std::make_pair(++m_highestID, std::make_unique<Actor>(name)));
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LOG_ERROR, exception.what());
		throw std::runtime_error("Failed to load Actor");
	}
}

void EditorScene::removeActor(int id) { m_actors.erase(id); }

void EditorScene::setSelected(int id) { m_selected = id; }

int EditorScene::getSelectedID() { return m_selected; }

Actor* EditorScene::getSelectedActor() {
	auto it = m_actors.find(m_selected);
	if(it != m_actors.end()) return it->second.get();
	return nullptr;
}
