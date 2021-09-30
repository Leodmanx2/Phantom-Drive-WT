#ifndef PD_SHADERPROGRAM_HPP
#define PD_SHADERPROGRAM_HPP

#include <globjects/base/File.h>
#include <globjects/globjects.h>
#include <memory>

class ShaderProgram {
	// While m_source is not used after construction, m_shader holds a
	// raw pointer to it and may segfault if it is not retained.
	// globjects::Shader cannot be configured not to track the shader
	// source.
	std::unique_ptr<globjects::File>    m_source;
	std::unique_ptr<globjects::Shader>  m_shader;
	std::unique_ptr<globjects::Program> m_program;

	public:
	explicit ShaderProgram(const gl::GLenum type, const std::string& file);

	globjects::Program& get() const;
	void                use() const;
	void                release();

	template <typename T>
	void setUniform(const std::string& name, const T& value) {
		m_program->setUniform(name, value);
	}

	template <typename T>
	void setUniform(gl::GLint location, const T& value) {
		m_program->setUniform(location, value);
	}

	operator globjects::Program*() const { return m_program.get(); }
};

#endif
