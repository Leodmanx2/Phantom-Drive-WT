#include "ShaderProgram.hpp"

#include <glbinding/gl/gl.h>

using namespace std;
using namespace gl;
using namespace globjects;

ShaderProgram::ShaderProgram(const gl::GLenum type,
                             const string&    file) // TODO: Take fs::path
  : m_source(nullptr), m_shader(nullptr), m_program(new Program()) {
	m_source = Shader::sourceFromFile(file);
	m_shader = make_unique<Shader>(type, m_source.get());
	m_program->attach(m_shader.get());
}

Program& ShaderProgram::get() const { return *m_program; }

void ShaderProgram::use() const { m_program->use(); }

void ShaderProgram::release() { m_program->release(); }
