#ifndef PD_SCENE_HPP
#define PD_SCENE_HPP

#include "Actor.hpp"
#include "Light.hpp"

#include <cstddef>
#include <memory>
#include <queue>
//#include <unordered_set>
#include <vector>

// Forward declarations ------------------------------------------------------
class Event {}; // TODO: Define Event
// ---------------------------------------------------------------------------

class Scene final {
	std::queue<Event> m_events;
	//std::unordered_set<Actor> m_actors;
	// TODO: Manage Actors

	// Lights
	std::vector<std::unique_ptr<Light>> m_pointLights;
	float                               m_ambience;

	void addObserver(const Actor& actor);
	void removeObserver(const Actor& actor);
	void notifyAll(); // Works through event queue, called by update()

	public:
	explicit Scene(const std::string& name);
	void queue(Event event);
	void update();
};

#endif
