#ifndef PD_SCENE_HPP
#define PD_SCENE_HPP

#define GLFW_INCLUDE_NONE

#include "Actor.hpp"
#include <array>
#include <cstddef>
#include <memory>
#include <queue>
#include <unordered_set>

// Forward declarations ------------------------------------------------------
struct PointLight;
struct SpotLight;
struct DirectionLight;
class Event;
// ---------------------------------------------------------------------------

class Scene final {
	std::queue<Event>         m_events;
	std::unordered_set<Actor> m_actors;

	// Lights
	std::array<std::unique_ptr<PointLight>, 8>     m_pointLights;
	std::array<std::unique_ptr<SpotLight>, 8>      m_spotLights;
	std::array<std::unique_ptr<DirectionLight>, 2> m_directionLights;
	float                                          m_ambience;

	void addObserver(const Actor& actor);
	void removeObserver(const Actor& actor);
	void notifyAll(); // Works through event queue, called by update()

	public:
	explicit Scene(const std::string& name);
	void queue(Event event);
	void update();
};

#endif
