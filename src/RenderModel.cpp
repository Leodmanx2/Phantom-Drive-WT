#include "RenderModel.h"

RenderModel::~RenderModel() {
	unsigned int vertexCount = sizeof(m_vertexBuffers)/sizeof(m_vertexBuffers[0]);
	unsigned int indexCount = sizeof(m_indexBuffers)/sizeof(m_indexBuffers[0]);
	unsigned int normalCount = sizeof(m_normalBuffers)/sizeof(m_normalBuffers[0]);
	unsigned int texCoordCount = sizeof(m_textureCoordBuffers)/sizeof(m_textureCoordBuffers[0]);
	
	// Release GPU resources
	glDeleteBuffers(vertexCount, m_vertexBuffers);
	glDeleteBuffers(indexCount, m_indexBuffers);
	glDeleteBuffers(normalCount, m_normalBuffers);
	glDeleteBuffers(texCoordCount, m_textureCoordBuffers);

	glDeleteProgram(m_shaderProgram);
	
	delete[] m_vertexBuffers;
	delete[] m_indexBuffers;
	delete[] m_normalBuffers;
	delete[] m_textureCoordBuffers;
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
