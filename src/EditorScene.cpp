#include "EditorScene.hpp"

#include "Actor.hpp"
#include "Light.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "input.hpp"
#include <plog/Log.h>

using namespace plog;

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
	} catch(const std::exception& exception) { LOG(error) << exception.what(); }

	PlCall("consult", {"SceneEdit.pro"});

	m_ambience = 0.2f;
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

		if(keyEvents.front().key == GLFW_KEY_GRAVE_ACCENT &&
		   keyEvents.front().action == GLFW_PRESS) {
			std::cout << "Enter command:\n";
			std::string command;
			std::getline(std::cin, command);
			PlTerm Codes;
			PlCall("atom_codes", {command.c_str(), Codes});

			PlTerm     T1, T2, T3;
			PlCompound pd_add_actor("pd_add_actor", {T1});
			PlCompound pd_move_by("pd_move_by", {T1, T2, T3});
			PlCompound pd_move_to("pd_move_to", {T1, T2, T3});
			PlCompound pd_rotate_by("pd_rotate_by", {T1, T2, T3});
			PlCompound pd_rotate_to("pd_rotate_to", {T1, T2, T3});
			PlCompound pd_select_light("pd_select_light", {T1});

			if(PlCall("phrase", {"pd_save", Codes})) {
				saveAs(name);
				std::cout << "success\n";
			} else if(PlCall("phrase", {"pd_exit", Codes})) {
				std::exit(EXIT_SUCCESS);
			} else if(PlCall("phrase", {pd_add_actor, Codes})) {
				try {
					addActor(static_cast<const char*>(T1));
					std::cout << "success\n";
				} catch(const std::exception& exception) {
					std::cout << "failure: " << exception.what();
				}
			} else if(PlCall("phrase", {"pd_select", Codes})) {
				const int selected =
				  Renderer::pick(Renderer::width() / 2, Renderer::height() / 2);
				std::cout << selected << "\n";
				setSelected(selected);
				std::cout << "success\n";
			} else if(PlCall("phrase", {"pd_deselect", Codes})) {
				setSelected(0);
				std::cout << "success\n";
			} else if(PlCall("phrase", {"pd_remove_actor", Codes})) {
				const int selected = selectedID();
				removeActor(selected);
				std::cout << "success\n";
			} else if(PlCall("phrase", {pd_move_by, Codes})) {
				if(selectedActor()) {
					selectedActor()->translate(static_cast<double>(T1),
					                           static_cast<double>(T2),
					                           static_cast<double>(T3));
					std::cout << "success\n";
				} else
					std::cout << "failure: no actor selected\n";
			} else if(PlCall("phrase", {pd_move_to, Codes})) {
				if(selectedActor()) {
					selectedActor()->setPosition(static_cast<double>(T1),
					                             static_cast<double>(T2),
					                             static_cast<double>(T3));
					std::cout << "success\n";
				} else
					std::cout << "failure: no actor selected\n";
			} else if(PlCall("phrase", {pd_rotate_by, Codes})) {
				if(selectedActor()) {
					selectedActor()->rotate(static_cast<double>(T1),
					                        static_cast<double>(T2),
					                        static_cast<double>(T3));
					std::cout << "success\n";
				} else
					std::cout << "failure: no actor selected\n";
			} else if(PlCall("phrase", {pd_rotate_to, Codes})) {
				if(selectedActor()) {
					const double    roll  = glm::radians(static_cast<double>(T1));
					const double    pitch = glm::radians(static_cast<double>(T2));
					const double    yaw   = glm::radians(static_cast<double>(T3));
					const glm::quat quat  = glm::quat(glm::vec3(pitch, yaw, roll));
					selectedActor()->setOrientation(quat.w, quat.x, quat.y, quat.z);
					std::cout << "success\n";
				} else
					std::cout << "failure: no actor selected\n";
			} else if(PlCall("phrase", {"pd_add_light", Codes})) {
				// TODO: add light command
				std::cout << "failure: not implemented\n";
			} else if(PlCall("phrase", {pd_select_light, Codes})) {
				// TODO: select light command
				std::cout << "failure: not implemented\n";
			} else if(PlCall("phrase", {"pd_remove_light", Codes})) {
				// TODO: remove light command
				std::cout << "failure: not implemented\n";
			} else if(PlCall("phrase", {"pd_edit_light", Codes})) {
				// TODO: edit light command
				std::cout << "failure: not implemented\n";
			} else {
				std::cout << "failure: unrecognized command\n";
			}
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
		LOG(error) << exception.what();
		throw std::runtime_error("Failed to load Actor");
	}
}

void EditorScene::removeActor(int id) { m_actors.erase(id); }
void EditorScene::setSelected(int id) { m_selected = id; }
int  EditorScene::selectedID() { return m_selected; }

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
