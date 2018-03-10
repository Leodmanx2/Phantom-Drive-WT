#ifndef SHADER_H
#define SHADER_H

#include <array>
#include <cassert>
#include <cstddef>
#include <glbinding/gl/gl.h>
#include <gli/gli.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <physfs.h>
#include <sstream>
#include <stdexcept>
#include <vector>

// Forward declarations ------------------------------------------------------
struct PointLight;
struct SpotLight;
struct DirectionLight;
// ---------------------------------------------------------------------------

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
	bool isBound() const;

	void setModelMatrix(const glm::mat4& matrix);
	void setViewMatrix(const glm::mat4& matrix);
	void setProjectionMatrix(const glm::mat4& matrix);

	void updateNormalMatrix();

	void setMatrices(const glm::mat4& model,
	                 const glm::mat4& view,
	                 const glm::mat4& projection);

	void setEyePosition(const glm::vec3& position);

	void setDiffuseMap(gl::GLuint id);
	void setSpecularMap(gl::GLuint id);

	void setAmbience(float ambience);

	void setPointLight(int index, const PointLight& light);
	void setSpotLight(int index, const SpotLight& light);
	void setDirectionLight(int index, const DirectionLight& light);

	void setObjectID(int objectID);
};

#endif
