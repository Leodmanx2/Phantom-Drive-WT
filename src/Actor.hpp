#ifndef ACTOR_H
#define ACTOR_H

#include "InputModel.hpp"
#include "Logger.hpp"
#include "RenderModel.hpp"
#include "Shader.hpp"
#include "SpatialModel.hpp"
#include <btBulletDynamicsCommon.h>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

class Actor final {
	struct ActorDescription final {
		const std::string ACTOR_DIR = "Actors/";

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

	void processInput(GLFWwindow& window);
	void draw(Shader& shader);
};

#endif
