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

void RenderModel::loadShaders(const char* vertexShaderFile, 
                              const char* pixelShaderFile, 
                              const char* geometryShaderFile) {
	// TODO: Use an interface to storage
	// Compile vertex shader
	std::ifstream vsFile(vertexShaderFile);
	std::stringstream vertexShaderSource;
	vertexShaderSource << vsFile.rdbuf();
	std::string vsSourceStr = vertexShaderSource.str();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vsSource = vsSourceStr.c_str();
	int vsSourceLength = vertexShaderSource.str().length();
	glShaderSource(vertexShader, 1, &vsSource, &vsSourceLength);
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
	std::ifstream psFile(pixelShaderFile);
	std::stringstream pixelShaderSource;
	pixelShaderSource << psFile.rdbuf();
	std::string psSourceStr = pixelShaderSource.str();
	unsigned int pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* psSource = psSourceStr.c_str();
	int psSourceLength = pixelShaderSource.str().length();
	glShaderSource(pixelShader, 1, &psSource, &psSourceLength);
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
	if(geometryShaderFile != NULL) {
		std::ifstream gsFile(geometryShaderFile);
		std::stringstream geometryShaderSource;
		geometryShaderSource << gsFile.rdbuf();
		std::string gsSourceStr = geometryShaderSource.str();
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		const char* gsSource = gsSourceStr.c_str();
		int gsSourceLength = geometryShaderSource.str().length();
		glShaderSource(geometryShader, 1, &gsSource, &gsSourceLength);
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
	if(geometryShaderFile != NULL) {
		glAttachShader(m_shaderProgram, geometryShader);
	}
	
	glLinkProgram(m_shaderProgram);
	
	int isLinked;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE) {
		int maxLength = 0;
		glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_shaderProgram, maxLength, &maxLength, &infoLog[0]);
		
		g_logger->write(Logger::ERROR, infoLog.data());
		
		glDeleteProgram(m_shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(pixelShader);
		if(geometryShaderFile != NULL) {
			glDeleteShader(geometryShader);
		}
		throw std::runtime_error("Failed to link shader program");
	}
	
	glDetachShader(m_shaderProgram, vertexShader);
	glDetachShader(m_shaderProgram, pixelShader);
	glDetachShader(m_shaderProgram, vertexShader);
	if(geometryShaderFile != NULL) {
		glDetachShader(m_shaderProgram, geometryShader);
	}
}
