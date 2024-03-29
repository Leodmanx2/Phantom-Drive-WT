#ifndef PD_SHADERPROGRAM_HPP
#define PD_SHADERPROGRAM_HPP

#include <globjects/base/File.h>
#include <globjects/globjects.h>
#include <memory>

// -------------
// ShaderProgram
// -------------

class ShaderProgram {
	// While m_source is not used after construction, m_shader holds a
	// raw pointer to it and may segfault if it is not retained.
	// globjects::Shader cannot be configured not to track the shader
	// source.
	std::unique_ptr<globjects::File>   m_source;
	std::unique_ptr<globjects::Shader> m_shader;

	protected:
	std::unique_ptr<globjects::Program> m_program;

	public:
	explicit ShaderProgram(const gl::GLenum type, const std::string& file);
	~ShaderProgram();

	constexpr globjects::Program* raw() const { return m_program.get(); };

	constexpr operator globjects::Program*() const { return raw(); }
};

// -------------------
// VertexShaderProgram
// -------------------

class VertexShaderProgram final : public ShaderProgram {
	public:
	explicit VertexShaderProgram(const std::string& file);

	void update_camera(const glm::mat4 view, const glm::vec3 eye);

	void transforms(const glm::mat4 model,
	                const glm::mat4 view,
	                const glm::mat4 projection);
};

// ---------------------
// FragmentShaderProgram
// ---------------------

class FragmentShaderProgram final : public ShaderProgram {
	public:
	static const gl::GLuint ALBEDO_TEXTURE_UNIT;
	static const gl::GLuint ROUGHNESS_TEXTURE_UNIT;
	static const gl::GLuint METALNESS_TEXTURE_UNIT;
	static const gl::GLuint OCCLUSION_TEXTURE_UNIT;
	static const gl::GLuint EMISSION_TEXTURE_UNIT;

	explicit FragmentShaderProgram(const std::string& file);

	void camera(const glm::mat4 view, const glm::vec3 eye);
};

#endif
