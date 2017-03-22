#ifndef SCENE_H
#define SCENE_H

#define GLFW_INCLUDE_NONE

#include "Actor.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <chrono>
#include <vector>

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

	virtual void update(std::chrono::milliseconds duration);
	virtual void simulate(std::chrono::milliseconds duration);
	virtual void draw();
	virtual void processInput(GLFWwindow& window);
};

#endif
