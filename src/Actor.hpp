#ifndef ACTOR_H
#define ACTOR_H

#include "InputModel.hpp"
#include "Logger.hpp"
#include "RenderModel.hpp"
#include "Shader.hpp"
#include "SpatialModel.hpp"
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

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

	void processInput(GLFWwindow& window);
	void draw(Shader& shader);

	// Forwarding functions
	constexpr void translate(float longitude, float latitude, float altitude) {
		m_spatialModel.translate(longitude, latitude, altitude);
	}

	constexpr void rotate(float roll, float pitch, float yaw) {
		m_spatialModel.rotate(roll, pitch, yaw);
	}

	constexpr void setOrientation(float deg, float x, float y, float z) {
		m_spatialModel.setOrientation(deg, x, y, z);
	}

	constexpr void setPosition(float x, float y, float z) {
		m_spatialModel.setPosition(x, y, z);
	}

	constexpr const glm::vec4& position() const {
		return m_spatialModel.position();
	}

	constexpr const glm::quat& orientation() const {
		return m_spatialModel.orientation();
	}
};

#endif
