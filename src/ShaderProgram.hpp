#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <globjects/globjects.h>
#include <memory>

class ShaderProgram {
	std::unique_ptr<globjects::Shader>  m_vertexShader;
	std::unique_ptr<globjects::Shader>  m_fragmentShader;
	std::unique_ptr<globjects::Program> m_program;

	public:
	explicit ShaderProgram(const std::string& name);
	globjects::Program* program() const;
};

#endif
