#include "ShaderProgram.hpp"

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace std;
using namespace gl;
using namespace globjects;

// -------------
// ShaderProgram
// -------------

ShaderProgram::ShaderProgram(const gl::GLenum type,
                             const string&    file) // TODO: Take fs::path
  : m_source(Shader::sourceFromFile(file))
  , m_shader(nullptr)
  , m_program(new Program()) {
	m_shader = make_unique<Shader>(type, m_source.get());
	m_program->attach(m_shader.get());
}

ShaderProgram::~ShaderProgram() { m_program->release(); }

// -------------------
// VertexShaderProgram
// -------------------

// TODO: Validate vertex shader has bindings required by renderer
VertexShaderProgram::VertexShaderProgram(const std::string& file)
  : ShaderProgram(GL_VERTEX_SHADER, file) {}

void VertexShaderProgram::transforms(const glm::mat4 model,
                                     const glm::mat4 view,
                                     const glm::mat4 projection) {
	m_program->setUniform("model", model);
	m_program->setUniform("view", view);
	m_program->setUniform("projection", projection);
	m_program->setUniform("normalMatrix", inverseTranspose(model * view));
}

// -------------------
// FragmentShaderProgram
// -------------------

// TODO: Validate fragment shader has bindings required by renderer
FragmentShaderProgram::FragmentShaderProgram(const std::string& file)
  : ShaderProgram(GL_FRAGMENT_SHADER, file) {
	m_program->setUniform("albedoMap", ALBEDO_TEXTURE_UNIT);
	m_program->setUniform("roughnessMap", ROUGHNESS_TEXTURE_UNIT);
	m_program->setUniform("metalnessMap", METALNESS_TEXTURE_UNIT);
	m_program->setUniform("occlusionMap", OCCLUSION_TEXTURE_UNIT);
	m_program->setUniform("emissionMap", EMISSION_TEXTURE_UNIT);
}

void FragmentShaderProgram::camera(const glm::mat4 view, const glm::vec3 eye) {
	m_program->setUniform("view", view);
	m_program->setUniform("eyePos", eye);
}
