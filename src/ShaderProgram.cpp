#include "ShaderProgram.hpp"

#include "utility.hpp"
#include <glbinding/gl/gl.h>

using namespace std;
using namespace gl;
using namespace globjects;

ShaderProgram::ShaderProgram(const string& name) : m_program() {
	string vertexSource = readFile(name + ".vert.glsl"s);
	m_vertexSource      = Shader::sourceFromString(vertexSource);
	m_vertexShader = make_unique<Shader>(GL_VERTEX_SHADER, m_vertexSource.get());

	string fragmentSource = readFile(name + ".frag.glsl"s);
	m_fragmentSource      = Shader::sourceFromString(fragmentSource);
	m_fragmentShader =
	  make_unique<Shader>(GL_FRAGMENT_SHADER, m_fragmentSource.get());

	m_program->attach(m_vertexShader.get());
	m_program->attach(m_fragmentShader.get());
}

Program& ShaderProgram::get() const { return *m_program; }
