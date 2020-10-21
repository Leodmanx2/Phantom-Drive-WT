#include "ShaderProgram.hpp"

#include <glbinding/gl/gl.h>
#include <globjects/base/File.h>

using namespace std;
using namespace gl;
using namespace globjects;

ShaderProgram::ShaderProgram(const string& name)
  : m_vertexShader(nullptr)
  , m_fragmentShader(nullptr)
  , m_program(new Program()) {
	auto vertexSource   = Shader::sourceFromFile(name + ".vert.glsl"s);
	auto fragmentSource = Shader::sourceFromFile(name + ".frag.glsl"s);
	m_vertexShader = make_unique<Shader>(GL_VERTEX_SHADER, vertexSource.get());
	m_fragmentShader =
	  make_unique<Shader>(GL_FRAGMENT_SHADER, fragmentSource.get());
	m_program->attach(m_vertexShader.get());
	m_program->attach(m_fragmentShader.get());
}

Program& ShaderProgram::get() const { return *m_program; }
