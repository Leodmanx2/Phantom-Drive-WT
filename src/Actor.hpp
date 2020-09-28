#ifndef PD_ACTOR_HPP
#define PD_ACTOR_HPP

#include "RenderComponent.hpp"
#include "SpatialComponent.hpp"
#include <memory>
#include <string>

class Event;

class Actor final {
	const std::string ACTOR_DIR = "Actors/";

	std::unique_ptr<RenderComponent>  m_renderComponent;
	std::unique_ptr<SpatialComponent> m_spatialComponent;

	public:
	explicit Actor(const std::string& name);
	Event onNotify(const Actor& source, Event event);
};

#endif
