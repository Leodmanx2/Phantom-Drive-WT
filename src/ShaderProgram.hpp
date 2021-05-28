#ifndef PD_SHADERPROGRAM_HPP
#define PD_SHADERPROGRAM_HPP

#include <globjects/base/File.h>
#include <globjects/globjects.h>
#include <memory>

class ShaderProgram {
	std::unique_ptr<globjects::File>    m_source;
	std::unique_ptr<globjects::Shader>  m_shader;
	std::unique_ptr<globjects::Program> m_program;

	public:
	explicit ShaderProgram(const gl::GLenum type, const std::string& file);

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
