#include "RenderModel.h"

RenderModel::~RenderModel() {
	unsigned int vertexCount = sizeof(m_vertexBuffers)/sizeof(m_vertexBuffers[0]);
	unsigned int indexCount = sizeof(m_indexBuffers)/sizeof(m_indexBuffers[0]);
	unsigned int normalCount = sizeof(m_normalBuffers)/sizeof(m_normalBuffers[0]);
	unsigned int texCoordCount = sizeof(m_textureCoordBuffers)/sizeof(m_textureCoordBuffers[0]);
	unsigned int samplerCount = sizeof(m_samplers)/sizeof(m_samplers[0]);
	
	// Release GPU resources
	glDeleteBuffers(vertexCount, m_vertexBuffers);
	glDeleteBuffers(indexCount, m_indexBuffers);
	glDeleteBuffers(normalCount, m_normalBuffers);
	glDeleteBuffers(texCoordCount, m_textureCoordBuffers);
	glDeleteSamplers(samplerCount, m_samplers);

	glDeleteProgram(m_shaderProgram);
	
	delete[] m_vertexBuffers;
	delete[] m_indexBuffers;
	delete[] m_normalBuffers;
	delete[] m_textureCoordBuffers;
	delete[] m_samplers;
}

void RenderModel::loadShaders(const char* vertexShaderFilename, 
                              const char* pixelShaderFilename, 
                              const char* geometryShaderFilename) {
	if(!PHYSFS_exists(vertexShaderFilename)) {
		throw std::runtime_error(std::string("Could not find vertex shader: ") + vertexShaderFilename);
	}
	if(!PHYSFS_exists(pixelShaderFilename)) {
		throw std::runtime_error(std::string("Could not find pixel shader: ") + pixelShaderFilename);
	}
	if(geometryShaderFilename != nullptr && !PHYSFS_exists(geometryShaderFilename)) {
		throw std::runtime_error(std::string("Could not find geometry shader: ") + geometryShaderFilename);
	}
	
	// NOTE: It may be possible to find the size by seeking if 
	//       PHYSFS_fileLength fails
	
	// Compile vertex shader
	PHYSFS_File* vertexShaderFile = PHYSFS_openRead(vertexShaderFilename);
	if(!vertexShaderFile) {
		throw std::runtime_error(std::string("Could not open vertex shader: ") + vertexShaderFilename);
	}
	
	PHYSFS_sint64 vsFileSizeLong = PHYSFS_fileLength(vertexShaderFile);
	if(vsFileSizeLong == -1)
		throw std::runtime_error(std::string("Could not determine size of vertex shader: ") + vertexShaderFilename);
	if(vsFileSizeLong > std::numeric_limits<int>::max())
		throw std::runtime_error(std::string("Vertex shader too large: ") + vertexShaderFilename);
	
	int vsFileSize = (int)vsFileSizeLong;
	
	char* vsBuffer = new char[vsFileSize];
	int vsBytesRead = PHYSFS_read(vertexShaderFile, vsBuffer, 1, vsFileSize);
	PHYSFS_close(vertexShaderFile);
	if(vsBytesRead < vsFileSize || vsBytesRead == -1) {
		delete[] vsBuffer;
		g_logger->write(Logger::ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of vertex shader: ") + vertexShaderFilename);
	}
	
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShader, 1, &vsBuffer, &vsFileSize);
	delete vsBuffer;
	glCompileShader(vertexShader);
	
	int isVSCompiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVSCompiled);
	if(isVSCompiled == GL_FALSE) {
		int maxLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		
		g_logger->write(Logger::ERROR, infoLog.data());
		
		glDeleteShader(vertexShader);
		throw std::runtime_error("Failed to compile vertex shader");
	}
	
	
	// Compile pixel shader
	PHYSFS_File* pixelShaderFile = PHYSFS_openRead(pixelShaderFilename);
	if(!pixelShaderFile) {
		throw std::runtime_error(std::string("Could not open pixel shader: ") + pixelShaderFilename);
	}
	
	PHYSFS_sint64 psFileSizeLong = PHYSFS_fileLength(pixelShaderFile);
	if(psFileSizeLong == -1)
		throw std::runtime_error(std::string("Could not determine size of pixel shader: ") + pixelShaderFilename);
	if(psFileSizeLong > std::numeric_limits<int>::max())
		throw std::runtime_error(std::string("Pixel shader too large: ") + pixelShaderFilename);
	
	int psFileSize = (int)psFileSizeLong;
	
	char* psBuffer = new char[psFileSize];
	int psBytesRead = PHYSFS_read(pixelShaderFile, psBuffer, 1, psFileSize);
	PHYSFS_close(pixelShaderFile);
	if(psBytesRead < psFileSize || psBytesRead == -1) {
		delete[] psBuffer;
		g_logger->write(Logger::ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of pixel shader: ") + pixelShaderFilename);
	}
	
	unsigned int pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(pixelShader, 1, &psBuffer, &psFileSize);
	delete psBuffer;
	glCompileShader(pixelShader);
	
	int isPSCompiled;
	glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &isPSCompiled);
	if(isPSCompiled == GL_FALSE) {
		int maxLength;
		glGetShaderiv(pixelShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(pixelShader, maxLength, &maxLength, &infoLog[0]);
		
		g_logger->write(Logger::ERROR, infoLog.data());
		
		glDeleteShader(pixelShader);
		throw std::runtime_error("Failed to compile pixel shader");
	}
	
	
	// Compile geometry shader
	unsigned int geometryShader;
	if(geometryShaderFilename != nullptr) {
		PHYSFS_File* geometryShaderFile = PHYSFS_openRead(geometryShaderFilename);
		if(!geometryShaderFile) {
			throw std::runtime_error(std::string("Could not open geometry shader: ") + geometryShaderFilename);
		}
		
		PHYSFS_sint64 gsFileSizeLong = PHYSFS_fileLength(geometryShaderFile);
		if(gsFileSizeLong == -1)
			throw std::runtime_error(std::string("Could not determine size of geometry shader: ") + geometryShaderFilename);
		if(gsFileSizeLong > std::numeric_limits<int>::max())
			throw std::runtime_error(std::string("Geometry shader too large: ") + geometryShaderFilename);
		
		int gsFileSize = (int)gsFileSizeLong;
		
		char* gsBuffer = new char[gsFileSize];
		int gsBytesRead = PHYSFS_read(geometryShaderFile, gsBuffer, 1, gsFileSize);
		PHYSFS_close(geometryShaderFile);
		if(gsBytesRead < gsFileSize || gsBytesRead == -1) {
			delete[] gsBuffer;
			g_logger->write(Logger::ERROR, PHYSFS_getLastError());
			throw std::runtime_error(std::string("Could not read all of geometry shader: ") + geometryShaderFilename);
		}
		
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		
		glShaderSource(vertexShader, 1, &gsBuffer, &gsFileSize);
		delete gsBuffer;
		glCompileShader(geometryShader);
		
		int isGSCompiled;
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &isGSCompiled);
		if(isGSCompiled == GL_FALSE) {
			int maxLength;
			glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(geometryShader, maxLength, &maxLength, &infoLog[0]);
			
			g_logger->write(Logger::ERROR, infoLog.data());
			
			glDeleteShader(geometryShader);
			throw std::runtime_error("Failed to compile geometry shader");
		}
	}
	
	
	// Link shaders
	m_shaderProgram = glCreateProgram();
	
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, pixelShader);
	if(geometryShaderFilename != nullptr) {
		glAttachShader(m_shaderProgram, geometryShader);
	}
	
	glLinkProgram(m_shaderProgram);
	
	int isLinked;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE) {
		int maxLength = 0;
		glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_shaderProgram, maxLength, &maxLength, &infoLog[0]);
		
		g_logger->write(Logger::ERROR, infoLog.data());
		
		glDeleteProgram(m_shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(pixelShader);
		if(geometryShaderFilename != nullptr) {
			glDeleteShader(geometryShader);
		}
		throw std::runtime_error("Failed to link shader program");
	}
	
	glDetachShader(m_shaderProgram, vertexShader);
	glDetachShader(m_shaderProgram, pixelShader);
	if(geometryShaderFilename != nullptr) {
		glDetachShader(m_shaderProgram, geometryShader);
	}
}

unsigned int RenderModel::loadDDSTextureToGPU(const char* filename, int* baseWidth, int* baseHeight) {
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
	
	// TODO: We need to check whether this is a valid DDS image
	gli::texture2D texture(gli::load_dds(buffer, fileSize));
	delete buffer;
	
	*baseWidth = texture[0].dimensions().x;
	*baseHeight = texture[0].dimensions().y;
	
	gli::gl gl;
	const gli::gl::format format = gl.translate(texture.format());
	
	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, texture.levels()-1);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, format.Swizzle[0]);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, format.Swizzle[1]);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, format.Swizzle[2]);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, format.Swizzle[3]);
	
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 
	               texture.levels(), 
	               format.Internal, 
	               texture.dimensions().x,
	               texture.dimensions().y,
	               1);
	
	if(gli::is_compressed(texture.format())) {
		for(std::size_t level=0; level<texture.levels(); ++level) {
			glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, 
			                          level, 
			                          0, 
			                          0, 
			                          0, 
			                          texture[level].dimensions().x, 
			                          texture[level].dimensions().y, 
			                          1, 
			                          format.External, 
			                          texture[level].size(),
			                          texture[level].data());
		}
	}
	else {
		for(std::size_t level=0; level<texture.levels(); ++level) {
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 
			level, 
			0, 
			0, 
			0,
			texture[level].dimensions().x, 
			texture[level].dimensions().y, 
			1, 
			format.External, 
			format.Type, 
			texture[level].data());
		}
	}
	
	return textureID;
}
