#ifndef PD_SHADERPROGRAM_HPP
#define PD_SHADERPROGRAM_HPP

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

	// TODO: Eliminate forwarding functions
	// Either fork glObjects and change how it tracks resources, or let
	// RenderComponents track individual vertex/geometry/fragment shaders

	template <typename T>
	void setUniform(const std::string& name, const T& value) {
		m_program->setUniform(name, value);
	}

	template <typename T>
	void setUniform(gl::GLint location, const T& value) {
		m_program->setUniform(location, value);
	}

	void use() const { m_program->use(); }
	void release() { m_program->release(); }
};

#endif
