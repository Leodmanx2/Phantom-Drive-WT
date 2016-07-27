#include "Shader.hpp"

Shader::Shader(const std::string& vertexShaderFilename,
               const std::string& pixelShaderFilename,
               const std::string& geometryShaderFilename)
  : m_active(false) {
	// Compile program
	try {
		loadShaders("Shaders/" + vertexShaderFilename,
		            "Shaders/" + pixelShaderFilename,
		            "Shaders/" + geometryShaderFilename);
	} catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());

		std::stringstream message;
		message << "Could not load shader (" << this << ")";
		std::string messageString = message.str();
		throw std::runtime_error(messageString);
	}

	bind();

	int positionAttrib = glGetAttribLocation(m_id, "position");
	if(positionAttrib != 0) {
		std::stringstream message;
		message << "Shader position attribute not at 0. Position obtained: "
		        << positionAttrib;
		throw std::logic_error(message.str());
	}

	int normalAttrib = glGetAttribLocation(m_id, "normal");
	if(normalAttrib != 1) {
		std::stringstream message;
		message << "Shader normal attribute not at 1. Position obtained: "
		        << normalAttrib;
		throw std::logic_error(message.str());
	}

	int texCoordAttrib = glGetAttribLocation(m_id, "texCoord");
	if(texCoordAttrib != 2) {
		std::stringstream message;
		message << "Shader texCoord attribute not at 2. Position obtained: "
		        << texCoordAttrib;
		throw std::logic_error(message.str());
	}

	// Save uniform locations
	m_modelUniform       = glGetUniformLocation(m_id, "model");
	m_viewUniform        = glGetUniformLocation(m_id, "view");
	m_normalUniform      = glGetUniformLocation(m_id, "normalMatrix");
	m_projectionUniform  = glGetUniformLocation(m_id, "projection");
	m_diffuseUniform     = glGetUniformLocation(m_id, "diffuseMap");
	m_specularUniform    = glGetUniformLocation(m_id, "specularMap");
	m_ambienceUniform    = glGetUniformLocation(m_id, "ambience");
	m_eyePositionUniform = glGetUniformLocation(m_id, "eyePos");

	for(size_t i = 0; i < m_pointLightUniforms.size(); ++i) {
		PointLightUniform& light = m_pointLightUniforms.at(i);

		std::stringstream positionStream;
		positionStream << "pointLights[" << i << "].position";
		const std::string position = positionStream.str();

		std::stringstream colorStream;
		colorStream << "pointLights[" << i << "].color";
		const std::string color = colorStream.str();

		std::stringstream intensityStream;
		intensityStream << "pointLights[" << i << "].intensity";
		const std::string intensity = intensityStream.str();

		std::stringstream radiusStream;
		radiusStream << "pointLights[" << i << "].radius";
		const std::string radius = radiusStream.str();

		light.position  = glGetUniformLocation(m_id, position.c_str());
		light.color     = glGetUniformLocation(m_id, color.c_str());
		light.intensity = glGetUniformLocation(m_id, intensity.c_str());
		light.radius    = glGetUniformLocation(m_id, radius.c_str());
	}

	for(size_t i = 0; i < m_spotLightUniforms.size(); ++i) {
		SpotLightUniform& light = m_spotLightUniforms.at(i);

		std::stringstream positionStream;
		positionStream << "spotLights[" << i << "].position";
		const std::string position = positionStream.str();

		std::stringstream directionStream;
		directionStream << "spotLights[" << i << "].direction";
		const std::string direction = directionStream.str();

		std::stringstream colorStream;
		colorStream << "spotLights[" << i << "].color";
		const std::string color = colorStream.str();

		std::stringstream intensityStream;
		intensityStream << "spotLights[" << i << "].intensity";
		const std::string intensity = intensityStream.str();

		std::stringstream angleStream;
		angleStream << "spotLights[" << i << "].angle";
		const std::string angle = angleStream.str();

		std::stringstream radiusStream;
		radiusStream << "spotLights[" << i << "].radius";
		const std::string radius = radiusStream.str();

		light.position  = glGetUniformLocation(m_id, position.c_str());
		light.direction = glGetUniformLocation(m_id, direction.c_str());
		light.color     = glGetUniformLocation(m_id, color.c_str());
		light.intensity = glGetUniformLocation(m_id, intensity.c_str());
		light.angle     = glGetUniformLocation(m_id, angle.c_str());
		light.radius    = glGetUniformLocation(m_id, radius.c_str());
	}

	for(size_t i = 0; i < m_directionLightUniforms.size(); ++i) {
		DirectionLightUniform& light = m_directionLightUniforms.at(i);

		std::stringstream directionStream;
		directionStream << "directionLights[" << i << "].direction";
		const std::string direction = directionStream.str();

		std::stringstream colorStream;
		colorStream << "directionLights[" << i << "].color";
		const std::string color = colorStream.str();

		std::stringstream intensityStream;
		intensityStream << "directionLights[" << i << "].intensity";
		const std::string intensity = intensityStream.str();

		light.direction = glGetUniformLocation(m_id, direction.c_str());
		light.color     = glGetUniformLocation(m_id, color.c_str());
		light.intensity = glGetUniformLocation(m_id, intensity.c_str());
	}
}

Shader::~Shader() { glDeleteProgram(m_id); }

/**
 * Compiles and links GLSL shader source files
 *
 * @param [in] vertexShaderFilename    Relative path to the vertex shader source file in the mounted asset location
 * @param [in] pixelShaderFilename     Relative path to the pixel/fragment source file in the mounted asset location
 * @param [in] geometryShaderFilename  Optional. Relative path to the geometry shader source file in the mounted asset location
 */
void Shader::loadShaders(const std::string& vertexShaderFilename,
                         const std::string& pixelShaderFilename,
                         const std::string& geometryShaderFilename) {
	unsigned int vertexShader;
	unsigned int pixelShader;
	unsigned int geometryShader;

	try {
		vertexShader = compileShader(vertexShaderFilename, GL_VERTEX_SHADER);
	} catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not load vertex shader");
	}

	try {
		pixelShader = compileShader(pixelShaderFilename, GL_FRAGMENT_SHADER);
	} catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not load pixel shader");
	}

	if(geometryShaderFilename.compare(SHADER_DIR) != 0) {
		try {
			geometryShader =
			  compileShader(geometryShaderFilename, GL_GEOMETRY_SHADER);
		} catch(const std::exception& exception) {
			g_logger->write(Logger::ERROR, exception.what());
			throw std::runtime_error("Could not load geometry shader");
		}
	}

	try {
		if(geometryShaderFilename.compare(SHADER_DIR) != 0)
			m_id = linkShaders(vertexShader, pixelShader, geometryShader);
		else
			m_id = linkShaders(vertexShader, pixelShader);
	} catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not link shader program");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);
	if(geometryShaderFilename.compare(SHADER_DIR) != 0)
		glDeleteShader(geometryShader);
}

/**
 * Compiles a shader source file into an in-memory object handled by OpenGL
 *
 * @param [in] filename  Relative path to the shader in the mounted asset location
 * @param [in] type      One of GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, or GL_GEOMETRY_SHADER
 *
 * @return OpenGL id referencing the compiled shader object
 */
unsigned int Shader::compileShader(const std::string& filename, GLenum type) {
	if(!PHYSFS_exists(filename.c_str())) {
		throw std::runtime_error(std::string("Could not find shader: ") + filename);
	}

	PHYSFS_File* shaderFile = PHYSFS_openRead(filename.c_str());
	if(!shaderFile) {
		throw std::runtime_error(std::string("Could not open shader: ") + filename);
	}

	PHYSFS_sint64 fileSizeLong = PHYSFS_fileLength(shaderFile);
	if(fileSizeLong == -1)
		throw std::runtime_error(
		  std::string("Could not determine size of shader: ") + filename);
	if(fileSizeLong > std::numeric_limits<int>::max())
		throw std::runtime_error(std::string("Shader too large: ") + filename);

	int fileSize = static_cast<int>(fileSizeLong);

	// TODO: Avoid manual memory management mid-function
	char* buffer    = new char[fileSize];
	int   bytesRead = PHYSFS_read(shaderFile, buffer, 1, fileSize);
	PHYSFS_close(shaderFile);
	if(bytesRead < fileSize || bytesRead == -1) {
		delete[] buffer;
		g_logger->write(Logger::ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of shader: ") +
		                         filename);
	}

	unsigned int id = glCreateShader(type);

	glShaderSource(id, 1, &buffer, &fileSize);
	delete[] buffer;
	glCompileShader(id);

	int isCompiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE) {
		int maxLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

		g_logger->write(Logger::ERROR, infoLog.data());

		glDeleteShader(id);
		throw std::runtime_error(std::string("Failed to compile shader: ") +
		                         filename);
	}

	return id;
}

/**
 * Links a compiled vertex shader, pixel shader, and geometry shader into a shader program.
 * Does not attempt to destroy the compiled shader objects.
 *
 * @param [in] vertexShader    OpenGL id referencing a compiled vertex shader object
 * @param [in] pixelShader     OpenGL id referencing a compiled pixel shader object
 * @param [in] geometryShader  OpenGL id referencing a compile geometry shader object
 *
 * @return OpenGL id referencing the linked shader program
 */
unsigned int Shader::linkShaders(unsigned int vertexShader,
                                 unsigned int pixelShader,
                                 unsigned int geometryShader) {
	unsigned int id = glCreateProgram();

	glAttachShader(id, vertexShader);
	glAttachShader(id, pixelShader);
	glAttachShader(id, geometryShader);

	glLinkProgram(id);

	int isLinked;
	glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE) {
		int maxLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

		g_logger->write(Logger::ERROR, infoLog.data());

		throw std::runtime_error("Failed to link shader program");
	}

	glDetachShader(id, vertexShader);
	glDetachShader(id, pixelShader);
	glDetachShader(id, geometryShader);

	return id;
}

/**
 * Links a compiled vertex shader and pixel shader into a shader program.
 * Does not attempt to destroy the compiled shader objects.
 *
 * @param [in] vertexShader    OpenGL id referencing a compiled vertex shader object
 * @param [in] pixelShader     OpenGL id referencing a compiled pixel shader object
 *
 * @return OpenGL id referencing the linked shader program
 */
unsigned int Shader::linkShaders(unsigned int vertexShader,
                                 unsigned int pixelShader) {
	unsigned int id = glCreateProgram();

	glAttachShader(id, vertexShader);
	glAttachShader(id, pixelShader);

	glLinkProgram(id);

	int isLinked;
	glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE) {
		int maxLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

		g_logger->write(Logger::ERROR, infoLog.data());

		throw std::runtime_error("Failed to link shader program");
	}

	glDetachShader(id, vertexShader);
	glDetachShader(id, pixelShader);

	return id;
}

void Shader::bind() {
	if(!m_active) glUseProgram(m_id);
}

void Shader::unbind() {
	if(m_active) glUseProgram(0);
}

bool Shader::isBound() { return m_active; }

void Shader::setModelMatrix(glm::mat4 matrix) {
	m_modelMatrix = matrix;
	glUniformMatrix4fv(m_modelUniform, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setViewMatrix(glm::mat4 matrix) {
	m_viewMatrix = matrix;
	glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setProjectionMatrix(glm::mat4 matrix) {
	glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::updateNormalMatrix() {
	glm::mat4 normalMatrix = glm::inverseTranspose(m_modelMatrix * m_viewMatrix);
	glUniformMatrix4fv(
	  m_normalUniform, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void Shader::setMatrices(glm::mat4 model,
                         glm::mat4 view,
                         glm::mat4 projection) {
	setModelMatrix(model);
	setViewMatrix(view);
	setProjectionMatrix(projection);
	updateNormalMatrix();
}

void Shader::setEyePosition(glm::vec3 position) {
	glUniform3fv(m_eyePositionUniform, 1, glm::value_ptr(position));
}

void Shader::setDiffuseMap(unsigned int id) {
	glUniform1i(m_diffuseUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Shader::setSpecularMap(unsigned int id) {
	glUniform1i(m_specularUniform, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Shader::setAmbience(float ambience) {
	glUniform1f(m_ambienceUniform, ambience);
}

void Shader::setPointLight(int index, PointLight& light) {
	assert(index >= 0 && index <= 8);
	glUniform3fv(
	  m_pointLightUniforms[index].position, 1, glm::value_ptr(light.position));
	glUniform3fv(
	  m_pointLightUniforms[index].color, 1, glm::value_ptr(light.color));
	glUniform1f(m_pointLightUniforms[index].intensity, light.intensity);
	glUniform1f(m_pointLightUniforms[index].radius, light.radius);
}

void Shader::setSpotLight(int index, SpotLight& light) {
	assert(index >= 0 && index <= 8);
	glUniform3fv(
	  m_spotLightUniforms[index].position, 1, glm::value_ptr(light.position));
	glUniform3fv(
	  m_spotLightUniforms[index].color, 1, glm::value_ptr(light.color));
	glUniform1f(m_spotLightUniforms[index].intensity, light.intensity);
	glUniform1f(m_spotLightUniforms[index].radius, light.radius);
	glUniform3fv(
	  m_spotLightUniforms[index].direction, 1, glm::value_ptr(light.direction));
	glUniform1f(m_spotLightUniforms[index].angle, light.angle);
}

void Shader::setDirectionLight(int index, DirectionLight& light) {
	assert(index >= 0 && index <= 2);
	glUniform3fv(
	  m_directionLightUniforms[index].color, 1, glm::value_ptr(light.color));
	glUniform1f(m_directionLightUniforms[index].intensity, light.intensity);
	glUniform3fv(m_directionLightUniforms[index].direction,
	             1,
	             glm::value_ptr(light.direction));
}