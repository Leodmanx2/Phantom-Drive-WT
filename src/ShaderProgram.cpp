#include "ShaderProgram.hpp"

#include <glbinding/gl/gl.h>
#include <globjects/base/File.h>

using namespace std;
using namespace gl;
using namespace globjects;

ShaderProgram::ShaderProgram(const gl::GLenum type,
                             const string&    file) // TODO: Take fs::path
  : m_shader(nullptr), m_program(new Program()) {
	auto source = Shader::sourceFromFile(file);
	m_shader    = make_unique<Shader>(type, source.get());
	m_program->attach(m_shader.get());
}

Program& ShaderProgram::get() const { return *m_program; }
