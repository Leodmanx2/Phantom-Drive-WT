#include "RenderModel.h"

int RenderModel::m_instanceCount = 0;

RenderModel::RenderModel() {
	++m_instanceCount;
}

RenderModel::RenderModel(RenderModel& original) {
	++m_instanceCount;
}

RenderModel::~RenderModel() {
	--m_instanceCount;
	if(m_instanceCount == 0) {
		// Release GPU resources
		glDeleteVertexArrays(1, &m_vertexArray);
		glDeleteProgram(m_shaderProgram);
		glDeleteSamplers(1, &m_sampler);
		glDeleteBuffers(1, &m_texture);
		glDeleteBuffers(1, &m_vertexBuffer);
		glDeleteBuffers(1, &m_indexBuffer);
	}
}

void RenderModel::glSetup(unsigned int shaderProgram, 
                          std::vector<Vertex>& vertices, 
                          std::vector<unsigned int>& indices) {
	// Save uniform locations
	m_modelUniform = glGetUniformLocation(shaderProgram, "model");
	m_viewUniform = glGetUniformLocation(shaderProgram, "view");
	m_projectionUniform = glGetUniformLocation(shaderProgram, "projection");
	m_textureUniform = glGetUniformLocation(shaderProgram, "textureSampler");
	
	// Reserve buffer IDs
	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_indexBuffer);
	
	// Fill buffers on the GPU using the prepared arrays we were passed
	glBindVertexArray(m_vertexArray);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), indices.data(), GL_STATIC_DRAW);
	
	// NOTE: It is possible that the shader doesn't actually use these values, in 
	//       which case they will be optimized out by the compiler and their 
	//       location will be returned as -1
	int positionAttribute = glGetAttribLocation(shaderProgram, "position");
	bool posAttribEnabled = positionAttribute == -1 ? false : true;
	if(posAttribEnabled) {
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		                      reinterpret_cast<void*>(offsetof(Vertex, position)));
		glEnableVertexAttribArray(positionAttribute);
	}

	int normalAttribute = glGetAttribLocation(shaderProgram, "normal");
	bool normAttribEnabled = normalAttribute == -1 ? false : true;
	if(normAttribEnabled) {
		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		                      reinterpret_cast<void*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(normalAttribute);
	}

	int texCoordAttribute = glGetAttribLocation(shaderProgram, "texCoord");
	bool texCoordAttribEnabled = texCoordAttribute == -1 ? false : true;
	if(texCoordAttribEnabled) {
		glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		                      reinterpret_cast<void*>(offsetof(Vertex, texCoord)));
		glEnableVertexAttribArray(texCoordAttribute);
	}
	
	// TODO: Everything related to animations and lights
	
	// Clean up
	glBindVertexArray(0);
}

/**
 * Compiles and links GLSL shader source files and registers the resulting program to be used for drawing this model
 *
 * @param [in] vertexShaderFilename    Relative path to the vertex shader source file in the mounted asset location
 * @param [in] pixelShaderFilename     Relative path to the pixel/fragment source file in the mounted asset location
 * @param [in] geometryShaderFilename  Optional. Relative path to the geometry shader source file in the mounted asset location
 */
void RenderModel::loadShaders(const char* vertexShaderFilename, 
                              const char* pixelShaderFilename, 
                              const char* geometryShaderFilename) {
	unsigned int vertexShader;
	unsigned int pixelShader;
	unsigned int geometryShader;
	
	try {vertexShader = compileShader(vertexShaderFilename, GL_VERTEX_SHADER);}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not load vertex shader");
	}
	
	try {pixelShader = compileShader(pixelShaderFilename, GL_FRAGMENT_SHADER);}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not load pixel shader");
	}
	
	if(geometryShaderFilename != nullptr) {
		try {geometryShader = compileShader(geometryShaderFilename, GL_GEOMETRY_SHADER);}
		catch(const std::exception& exception) {
			g_logger->write(Logger::ERROR, exception.what());
			throw std::runtime_error("Could not load geometry shader");
		}
	}
	
	try {
		if(geometryShaderFilename != nullptr)
			m_shaderProgram = linkShaders(vertexShader, pixelShader, geometryShader);
		else
			m_shaderProgram = linkShaders(vertexShader, pixelShader);
	}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not link shader program");
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);
	if(geometryShaderFilename != nullptr) glDeleteShader(geometryShader);
}

/**
 * Compiles a shader source file into an in-memory object handled by OpenGL
 *
 * @param [in] filename  Relative path to the shader in the mounted asset location
 * @param [in] type      One of GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, or GL_GEOMETRY_SHADER
 *
 * @return OpenGL id referencing the compiled shader object
 */
unsigned int RenderModel::compileShader(const char* filename, GLenum type) {
	if(!PHYSFS_exists(filename)) {
		throw std::runtime_error(std::string("Could not find shader: ") + filename);
	}
	
	PHYSFS_File* shaderFile = PHYSFS_openRead(filename);
	if(!shaderFile) {
		throw std::runtime_error(std::string("Could not open shader: ") + filename);
	}
	
	PHYSFS_sint64 fileSizeLong = PHYSFS_fileLength(shaderFile);
	if(fileSizeLong == -1)
		throw std::runtime_error(std::string("Could not determine size of shader: ") + filename);
	if(fileSizeLong > std::numeric_limits<int>::max())
		throw std::runtime_error(std::string("Shader too large: ") + filename);
	
	int fileSize = static_cast<int>(fileSizeLong);
	
	// TODO: Avoid manual memory management mid-function
	char* buffer = new char[fileSize];
	int bytesRead = PHYSFS_read(shaderFile, buffer, 1, fileSize);
	PHYSFS_close(shaderFile);
	if(bytesRead < fileSize || bytesRead == -1) {
		delete[] buffer;
		g_logger->write(Logger::ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of shader: ") + filename);
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
		throw std::runtime_error(std::string("Failed to compile shader: ") + filename);
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
unsigned int RenderModel::linkShaders(unsigned int vertexShader, 
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
unsigned int RenderModel::linkShaders(unsigned int vertexShader, 
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

/**
 * Parses a texture file into an OpenGL texture object.
 *
 * @param [in] filename     Relative path to the texture file in the mounted asset directory
 * @param [out] baseWidth   The width of the standard-detail image. Used as a hack to size render models.
 * @param [out] baseHeight  The height of the standard-detail image. Used as a hack to size render models.
 *
 * @return OpenGL id referencing the texture object in GPU memory
 */
unsigned int RenderModel::loadTextureToGPU(const char* filename, int* baseWidth, int* baseHeight) {
	// Use PhysFS to read texture file into memory
	if(std::string(filename).compare("") == 0 || !PHYSFS_exists(filename))
		throw std::runtime_error(std::string("Could not find texture: ") + filename);
	
	PHYSFS_File* file = PHYSFS_openRead(filename);
	if(!file)
		throw std::runtime_error(std::string("Could not open texture: ") + filename);
	
	PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
	if(fileSize == -1)
		throw std::runtime_error(std::string("Could not determine size of texture: ") + filename);
	
	char* buffer = new char[fileSize];
	int bytesRead = PHYSFS_read(file, buffer, 1, fileSize);
	PHYSFS_close(file);
	if(bytesRead < fileSize || bytesRead == -1) {
		delete[] buffer;
		g_logger->write(Logger::ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of texture: ") + filename);
	}
	
	// Create a GLI texture from the data read in by PhysFS
	gli::texture texture = gli::load(buffer, fileSize);
	delete[] buffer;
	gli::gl gl;
	const gli::gl::format format = gl.translate(texture.format());
	GLenum target = gl.translate(texture.target());
	if(target != GL_TEXTURE_2D) {
		throw std::runtime_error("Texture target is not GL_TEXTURE_2D/gli::TARGET_2D");
	}
	if(texture.empty()) {
		throw std::runtime_error(filename + std::string(" is not a valid texture"));
	}

	// We currently need to write these out in order to size
	// 2d render models. This should be written out in the
	// near future.
	*baseWidth = texture.dimensions().x;
	*baseHeight = texture.dimensions().y;
	
	// Reserve memory on the GPU for texture and describe its layout
	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	glBindTexture(target, textureID);
	
	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, texture.levels()-1);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzle[0]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzle[1]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzle[2]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzle[3]);
	
	glTexStorage2D(target, 
	               texture.levels(), 
	               format.Internal,
	               texture.dimensions().x, 
	               texture.dimensions().y);

	// Write image data to GPU memory
	for(unsigned int layer=0; layer<texture.layers(); ++layer) {
		for(unsigned int face=0; face<texture.faces(); ++face) {
			for(unsigned int level=0; level<texture.levels(); ++level) {
				if(gli::is_compressed(texture.format())) {
					glCompressedTexSubImage2D(target, 
					                          level, 
					                          0, 
					                          0, 
					                          texture.dimensions().x, 
					                          texture.dimensions().y, 
					                          format.Internal, 
					                          texture.size(level), 
					                          texture.data(layer, face, level));
				}
				else {
					glTexSubImage2D(target, 
					                level, 
					                0, 
					                0, 
					                texture.dimensions().x, 
					                texture.dimensions().y, 
					                format.External, 
					                format.Type, 
					                texture.data(layer, face, level));
				}
			}
		}
	}

	return textureID;
}
