#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <globjects/globjects.h>
#include <memory>

// TODO: Eliminate shader sources from memory
//       They are only being retained because globjects::Shader tracks them

class ShaderProgram {
	std::unique_ptr<globjects::AbstractStringSource> m_vertexSource;
	std::unique_ptr<globjects::AbstractStringSource> m_fragmentSource;
	std::unique_ptr<globjects::Shader>               m_vertexShader;
	std::unique_ptr<globjects::Shader>               m_fragmentShader;
	std::unique_ptr<globjects::Program>              m_program;

	public:
	explicit ShaderProgram(const std::string& name);
	globjects::Program* program() const;
};

#endif
