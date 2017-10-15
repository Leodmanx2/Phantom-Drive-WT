#include "EditorScene.hpp"

// ---------------------------------------------------------------------------
//  Prolog Interface to Scene Editing Commands
// ---------------------------------------------------------------------------
// TODO: Rewrite as editor member functions

PREDICATE(pd_save, 1) {
	EditorScene& scene = *static_cast<EditorScene*>(static_cast<void*>(A1));
	scene.saveAs(scene.name);
	return true;
}

PREDICATE(pd_exit, 1) { std::exit(EXIT_SUCCESS); }

PREDICATE(pd_add_actor, 2) {
	EditorScene& scene = *static_cast<EditorScene*>(static_cast<void*>(A1));
	try {
		scene.addActor(static_cast<const char*>(A2));
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, exception.what());
		return false;
	}
	return true;
}

PREDICATE(pd_select, 1) {
	EditorScene& scene = *static_cast<EditorScene*>(static_cast<void*>(A1));
	const int    selected =
	  Renderer::pick(Renderer::width() / 2, Renderer::height() / 2);
	std::cout << selected << "\n";
	scene.setSelected(selected);
	return true;
}

PREDICATE(pd_select, 3) {
	const int x = static_cast<int>(A1);
	const int y = static_cast<int>(A2);
	A3          = Renderer::pick(x, y);
	return true;
}

PREDICATE(pd_deselect, 1) {
	EditorScene& scene = *static_cast<EditorScene*>(static_cast<void*>(A1));
	scene.setSelected(0);
	return true;
}

PREDICATE(pd_remove_actor, 1) {
	EditorScene& scene    = *static_cast<EditorScene*>(static_cast<void*>(A1));
	const int    selected = scene.selectedID();
	scene.removeActor(selected);
	return true;
}

PREDICATE(pd_move_by, 4) {
	EditorScene& scene     = *static_cast<EditorScene*>(static_cast<void*>(A1));
	const double longitude = static_cast<double>(A2);
	const double latitude  = static_cast<double>(A3);
	const double altitude  = static_cast<double>(A4);
	Actor*       actor     = scene.selectedActor();
	if(!actor) return false;
	actor->translate(longitude, latitude, altitude);
	return true;
}

PREDICATE(pd_move_to, 4) {
	EditorScene& scene = *static_cast<EditorScene*>(static_cast<void*>(A1));
	const double x     = static_cast<double>(A2);
	const double y     = static_cast<double>(A3);
	const double z     = static_cast<double>(A4);
	Actor*       actor = scene.selectedActor();
	if(!actor) return false;
	actor->setPosition(x, y, z);
	return true;
}

PREDICATE(pd_rotate_by, 4) {
	EditorScene& scene = *static_cast<EditorScene*>(static_cast<void*>(A1));
	const double roll  = static_cast<double>(A2);
	const double pitch = static_cast<double>(A3);
	const double yaw   = static_cast<double>(A4);
	Actor*       actor = scene.selectedActor();
	if(!actor) return false;
	actor->rotate(roll, pitch, yaw);
	return true;
}

PREDICATE(pd_rotate_to, 4) {
	EditorScene& scene = *static_cast<EditorScene*>(static_cast<void*>(A1));
	const double roll  = glm::radians(static_cast<double>(A2));
	const double pitch = glm::radians(static_cast<double>(A3));
	const double yaw   = glm::radians(static_cast<double>(A4));
	Actor*       actor = scene.selectedActor();
	if(!actor) return false;
	const glm::quat quat = glm::quat(glm::vec3(pitch, yaw, roll));
	actor->setOrientation(quat.w, quat.x, quat.y, quat.z);
	return true;
}

PREDICATE(pd_add_light, 1) { return false; }

PREDICATE(pd_select_light, 2) { return false; }

PREDICATE(pd_remove_light, 1) { return false; }

PREDICATE(pd_edit_light, 1) { return false; }

// ---------------------------------------------------------------------------
//  Scene Overrides
// ---------------------------------------------------------------------------

EditorScene::EditorScene(const std::string& sceneName)
  : Scene(name), m_mutex(), m_selected(0), name(sceneName) {
	// TODO: Editor mode scene loading
	// The scene will load normally first.
	// If the scene file can't be found, we create a new one.
	// If the scene file is corrupt, we give up.
	try {
		m_defaultShader = new Shader("textured.vert.glsl", "textured.frag.glsl");
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, exception.what());
	}

	PlCall("consult", {"SceneEdit.pro"});

	m_ambience = 0.2f;
	addActor("Akari");
}

EditorScene::~EditorScene() { delete m_defaultShader; }

// The editor works with the initial moment of a scene.
// We don't want to do anything.
void EditorScene::update(const std::chrono::milliseconds&) {}
void EditorScene::simulate(const std::chrono::milliseconds&) {}

// Similarly, we don't want the Actors processing any input.
// Input processing is limited to editor and camera input.
void EditorScene::process(std::queue<KeyEvent>& keyEvents) {
	while(!keyEvents.empty()) {
		m_editorCamera.process(keyEvents.front());
		// TODO: Handle events for the scene on the C++ side
		if(keyEvents.front().key == GLFW_KEY_GRAVE_ACCENT &&
		   keyEvents.front().action == GLFW_PRESS) {
			std::cout << "Enter command:\n";
			std::string command;
			std::getline(std::cin, command);
			PlTerm Codes;
			PlCall("atom_codes", {command.c_str(), Codes});
			if(PlCall("phrase", {"pd_save", Codes})) {
				std::cout << "Save command!\n";
			} // TODO: check other commands
		}
		keyEvents.pop();
	}
}

void EditorScene::process(std::queue<MouseButtonEvent>& buttonEvents) {
	while(!buttonEvents.empty()) {
		m_editorCamera.process(buttonEvents.front());
		// TODO: Handle events for the scene on the C++ side
		buttonEvents.pop();
	}
}

void EditorScene::process(std::queue<MouseMovementEvent>& movementEvents) {
	while(!movementEvents.empty()) {
		m_editorCamera.process(movementEvents.front());
		// TODO: Handle events for the scene on the C++ side
		movementEvents.pop();
	}
}

void EditorScene::draw() {
	const glm::mat4 projectionMatrix =
	  glm::perspective(45.0f,
	                   static_cast<float>(Renderer::width()) /
	                     static_cast<float>(Renderer::height()),
	                   0.1f,
	                   10000.0f);
	for(auto actor = m_actors.begin(); actor != m_actors.end(); ++actor) {
		m_defaultShader->setAmbience(m_ambience);

		// Send camera position to the GPU.
		m_defaultShader->setViewMatrix(m_editorCamera.viewMatrix());
		m_defaultShader->setEyePosition(glm::vec3(m_editorCamera.position()));

		m_defaultShader->setProjectionMatrix(projectionMatrix);
		m_defaultShader->setObjectID(actor->first);
		actor->second->draw(*m_defaultShader);
	}
}

// ---------------------------------------------------------------------------
//  Editor Functionality
// ---------------------------------------------------------------------------

void EditorScene::addActor(const std::string& actorName) {
	try {
		m_actors.emplace(
		  std::make_pair(++m_highestID, std::make_unique<Actor>(actorName)));
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, exception.what());
		throw std::runtime_error("Failed to load Actor");
	}
}

Actor* EditorScene::selectedActor() {
	const auto it = m_actors.find(m_selected);
	if(it != m_actors.end()) return it->second.get();
	return nullptr;
}

void EditorScene::saveAs(const std::string& file) {
	// TODO: Seriously improve the format libraries, maybe move this out to an exporter class
	PSCN::File contents;

	// Header
	contents.header =
	  PSCN::Header(0x5053434e, 0); // TODO: This should be set by the library

	// Actors
	for(auto pair = m_actors.begin(); pair != m_actors.end(); ++pair) {
		const Actor& actor = *pair->second.get();
		PSCN::Actor  actorPSCN;
		// Name
		actorPSCN.name = actor.name();
		// Position
		const glm::vec4 pos = actor.position();
		actorPSCN.position  = {pos.x, pos.y, pos.z};
		// Orientation
		const glm::quat ori         = actor.orientation();
		const glm::vec3 eulerAngles = glm::eulerAngles(ori);
		actorPSCN.orientation       = {eulerAngles.z, eulerAngles.x, eulerAngles.y};

		contents.body.actors.push_back(actorPSCN);
	}

	// Cameras
	for(auto camera : m_cameras) {
		PSCN::Camera cameraPSCN;
		// Position
		const glm::vec4 pos = camera.position();
		cameraPSCN.position = {pos.x, pos.y, pos.z};
		// Orientation
		const glm::quat ori         = camera.orientation();
		const glm::vec3 eulerAngles = glm::eulerAngles(ori);
		cameraPSCN.orientation      = {eulerAngles.z, eulerAngles.x, eulerAngles.y};

		contents.body.cameras.push_back(cameraPSCN);
	}

	// PointLights
	for(auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
		if(!*it) continue;
		const PointLight& light = *it->get();
		PSCN::PointLight  lightPSCN;
		lightPSCN.position = {light.position.x, light.position.y, light.position.z};
		lightPSCN.color    = {light.color.x, light.color.y, light.color.z};
		lightPSCN.intensity = light.intensity;
		lightPSCN.radius    = light.radius;
		contents.body.pointLights.push_back(lightPSCN);
	}

	// SpotLights
	for(auto it = m_spotLights.begin(); it != m_spotLights.end(); ++it) {
		if(!*it) continue;
		const SpotLight& light = *it->get();
		PSCN::SpotLight  lightPSCN;
		lightPSCN.position = {light.position.x, light.position.y, light.position.z};
		lightPSCN.direction = {
		  light.direction.x, light.direction.y, light.direction.z};
		lightPSCN.color     = {light.color.x, light.color.y, light.color.z};
		lightPSCN.intensity = light.intensity;
		lightPSCN.angle     = light.angle;
		lightPSCN.radius    = light.radius;
		contents.body.spotLights.push_back(lightPSCN);
	}

	// DirectionalLights
	for(auto it = m_directionLights.begin(); it != m_directionLights.end();
	    ++it) {
		if(!*it) continue;
		const DirectionLight& light = *it->get();
		PSCN::DirectionLight  lightPSCN;
		lightPSCN.direction = {
		  light.direction.x, light.direction.y, light.direction.z};
		lightPSCN.color     = {light.color.x, light.color.y, light.color.z};
		lightPSCN.intensity = light.intensity;
		contents.body.directionLights.push_back(lightPSCN);
	}

	contents.write(file);
}
