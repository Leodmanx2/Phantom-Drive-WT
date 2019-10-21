#ifndef PD_SCENE_HPP
#define PD_SCENE_HPP

#define GLFW_INCLUDE_NONE

#include "Camera.hpp"
#include "Shader.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <chrono>
#include <cstddef>
#include <vector>

// Forward declarations ------------------------------------------------------
class Actor;
struct PointLight;
struct SpotLight;
struct DirectionLight;
struct KeyEvent;
struct MouseMovementEvent;
struct MouseButtonEvent;
// ---------------------------------------------------------------------------

class Scene {
	protected:
	std::vector<Shader> m_shaders;

	std::vector<Camera> m_cameras;

	std::map<std::uint32_t, std::unique_ptr<Actor>> m_actors;

	std::array<std::unique_ptr<PointLight>, 8>     m_pointLights;
	std::array<std::unique_ptr<SpotLight>, 8>      m_spotLights;
	std::array<std::unique_ptr<DirectionLight>, 2> m_directionLights;

	float m_ambience; // How bright the scene is on average

	std::uint32_t m_highestID; // TODO: Rename. Highest is a misleading term.

	public:
	explicit Scene(const std::string& name);
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual ~Scene();

	virtual void update(const std::chrono::milliseconds& duration);
	virtual void simulate(const std::chrono::milliseconds& duration);
	virtual void draw();
	virtual void process(std::queue<KeyEvent>& keyEvents);
	virtual void process(std::queue<MouseButtonEvent>& buttonEvents);
	virtual void process(std::queue<MouseMovementEvent>& movementEvents);
};

#endif
