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
  : ShaderProgram(GL_FRAGMENT_SHADER, file) {}

void FragmentShaderProgram::camera(const glm::mat4 view, const glm::vec3 eye) {
	m_program->setUniform("view", view);
	m_program->setUniform("eyePos", eye);
}

void FragmentShaderProgram::textures(const globjects::Texture* diffuse,
                                     const globjects::Texture* specular) {
	m_program->setUniform("diffuseMap", 0);
	diffuse->bindActive(0);
	m_program->setUniform("specularMap", 1);
	specular->bindActive(1);
}
