#include "ShaderProgram.hpp"

#include "utility.hpp"
#include <glbinding/gl/gl.h>
#include <globjects/base/StaticStringSource.h>

using namespace std;
using namespace gl;
using namespace globjects;

ShaderProgram::ShaderProgram(const string& name) : m_program() {
	string                         vertexSource = readFile(name + ".vert.glsl"s);
	unique_ptr<StaticStringSource> vss = Shader::sourceFromString(vertexSource);
	m_vertexShader = make_unique<Shader>(GL_VERTEX_SHADER, vss.get());

	string fragmentSource              = readFile(name + ".frag.glsl"s);
	unique_ptr<StaticStringSource> fss = Shader::sourceFromString(fragmentSource);
	m_fragmentShader = make_unique<Shader>(GL_FRAGMENT_SHADER, fss.get());

	m_program->attach(m_vertexShader.get());
	m_program->attach(m_fragmentShader.get());
}

Program* ShaderProgram::program() const { return m_program.get(); }
