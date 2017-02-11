#ifndef SHADER_H
#define SHADER_H

#include "Light.hpp"
#include "Logger.hpp"
#include "utility.hpp"
#include <array>
#include <cassert>
#include <glbinding/gl/gl.h>
#include <gli/gli.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <physfs.h>
#include <sstream>
#include <stdexcept>
#include <vector>

class Shader final {
	const std::string SHADER_DIR = "Shaders/";

	struct PointLightUniform {
		int position;
		int color;
		int intensity;
		int radius;
	};

	struct SpotLightUniform {
		int position;
		int direction;
		int color;
		int intensity;
		int angle;
		int radius;
	};

	struct DirectionLightUniform {
		int direction;
		int color;
		int intensity;
	};

	private:
	bool m_active;

	gl::GLuint m_id;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_viewMatrix;

	gl::GLint m_modelUniform;
	gl::GLint m_viewUniform;
	gl::GLint m_normalUniform;
	gl::GLint m_projectionUniform;

	gl::GLint m_eyePositionUniform;

	gl::GLint m_diffuseUniform;
	gl::GLint m_specularUniform;
	gl::GLint m_ambienceUniform;

	std::array<PointLightUniform, 8>     m_pointLightUniforms;
	std::array<SpotLightUniform, 8>      m_spotLightUniforms;
	std::array<DirectionLightUniform, 8> m_directionLightUniforms;

	gl::GLint m_objectIDUniform;

	void loadShaders(const std::string& vertexShaderFilename,
	                 const std::string& pixelShaderFilename,
	                 const std::string& geometryShaderFilename);

	gl::GLuint compileShader(const std::string& filename, gl::GLenum type);

	gl::GLuint linkShaders(gl::GLuint vertexShader,
	                       gl::GLuint pixelShader,
	                       gl::GLuint geometryShader);

	gl::GLuint linkShaders(gl::GLuint vertexShader, gl::GLuint pixelShader);

	public:
	Shader(const std::string& vertexShaderFilename,
	       const std::string& pixelShaderFilename,
	       const std::string& geometryShaderFilename = "");

	// Disable copying
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	// Enable moving
	Shader(Shader&&) = default;
	Shader& operator=(Shader&&) = default;

	~Shader();

	void bind();
	void unbind();
	bool isBound();

	void setModelMatrix(glm::mat4 matrix);
	void setViewMatrix(glm::mat4 matrix);
	void setProjectionMatrix(glm::mat4 matrix);

	void updateNormalMatrix();

	void setMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

	void setEyePosition(glm::vec3 position);

	void setDiffuseMap(gl::GLuint id);
	void setSpecularMap(gl::GLuint id);

	void setAmbience(float ambience);

	void setPointLight(int index, PointLight& light);
	void setSpotLight(int index, SpotLight& light);
	void setDirectionLight(int index, DirectionLight& light);

	void setObjectID(int objectID);
};

#endif
