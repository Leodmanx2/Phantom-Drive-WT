#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <globjects/globjects.h>
#include <memory>

// Users of ShaderProgram need the complete type info of StaticStringSource to
// construct instances.
#include <globjects/base/StaticStringSource.h>

// TODO: Eliminate shader sources from memory
//       They are only being retained because globjects::Shader tracks them

class ShaderProgram {
	std::unique_ptr<globjects::StaticStringSource> m_vertexSource;
	std::unique_ptr<globjects::StaticStringSource> m_fragmentSource;
	std::unique_ptr<globjects::Shader>             m_vertexShader;
	std::unique_ptr<globjects::Shader>             m_fragmentShader;
	std::unique_ptr<globjects::Program>            m_program;

	public:
	explicit ShaderProgram(const std::string& name);

	globjects::Program& get() const;
};

#endif
