#include "ShaderProgram.hpp"

#include <glbinding/gl/gl.h>
#include <globjects/base/File.h>

using namespace std;
using namespace gl;
using namespace globjects;

ShaderProgram::ShaderProgram(const string& name)
  : m_vertexSource(Shader::sourceFromFile(name + ".vert.glsl"s))
  , m_fragmentSource(Shader::sourceFromFile(name + ".frag.glsl"s))
  , m_vertexShader(new Shader(GL_VERTEX_SHADER, m_vertexSource.get()))
  , m_fragmentShader(new Shader(GL_FRAGMENT_SHADER, m_fragmentSource.get()))
  , m_program(new Program()) {
	m_program->attach(m_vertexShader.get());
	m_program->attach(m_fragmentShader.get());
}

Program& ShaderProgram::get() const { return *m_program; }
