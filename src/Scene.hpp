#ifndef SCENE_H
#define SCENE_H

#define GLFW_INCLUDE_NONE

#include "Actor.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include <vector>

class Scene {
	private:
	Shader* m_activeShader;

	Camera* m_activeCamera;

	std::vector<std::unique_ptr<Actor>> m_actors;

	// TODO: Remove specific actors, etc. These are for development purposes only.
	// Scene should not have details of specific scenes. Scenes must be built
	// from external descriptions, just like Shader and RenderModel.
	PointLight*     m_pointLight;
	SpotLight*      m_spotLight;
	DirectionLight* m_directionLight;

	float m_ambience; // How bright the scene is on average

	std::uint32_t m_highestID;

	public:
	Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	~Scene();

	void update(std::chrono::milliseconds duration);
	void simulate(std::chrono::milliseconds duration);
	void draw();
	void processInput(GLFWwindow& window);
};

#endif
