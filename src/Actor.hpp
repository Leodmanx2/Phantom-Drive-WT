#ifndef PD_ACTOR_HPP
#define PD_ACTOR_HPP

#include "InputModel.hpp"
#include "SpatialModel.hpp"
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <memory>

// Forward declarations ------------------------------------------------------
class RenderModel;
class Shader;
struct KeyEvent;
struct MouseMovementEvent;
struct MouseButtonEvent;
// ---------------------------------------------------------------------------

class Actor final {
	struct ActorDescription final {
		const std::string ACTOR_DIR = "Actors/";

		std::string name;
		std::string renderModel;
		std::string inputModel;

		explicit ActorDescription(const std::string& actorName);
	};

	// We maintain a dictionary of already-constructed render models so that if
	// we need to make a new Actor with one of them, we don't have to
	// reconstruct it.
	static std::map<std::string, std::shared_ptr<RenderModel>> s_modelDictionary;

	protected:
	ActorDescription m_desc;

	SpatialModel                 m_spatialModel;
	InputModel                   m_inputModel;
	std::shared_ptr<RenderModel> m_renderModel;

	public:
	explicit Actor(const std::string& name);

	Actor(const Actor& original) = default;
	Actor& operator=(const Actor& original) = default;

	Actor(Actor&& original) = default;
	Actor& operator=(Actor&& original) = default;

	~Actor();

	const std::string& name() const;

	void process(const KeyEvent& event);
	void process(const MouseButtonEvent& event);
	void process(const MouseMovementEvent& event);
	void draw(Shader& shader);

	// Forwarding functions
	void             translate(float longitude, float latitude, float altitude);
	void             rotate(float roll, float pitch, float yaw);
	void             setOrientation(float deg, float x, float y, float z);
	void             setPosition(float x, float y, float z);
	const glm::vec4& position() const;
	const glm::quat& orientation() const;
};

#endif
