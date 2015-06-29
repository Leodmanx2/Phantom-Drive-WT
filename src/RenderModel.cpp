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
	
	delete m_vertexBuffers;
	delete m_indexBuffers;
	delete m_normalBuffers;
	delete m_textureCoordBuffers;
}

void RenderModel::loadShaders(const char* vertexShaderFile, 
                              const char* pixelShaderFile, 
                              const char* geometryShaderFile) {
	// TODO: Use an interface to storage
	int isCompiled;
	
	// Compile vertex shader
	std::ifstream vsFile(vertexShaderFile);
	std::stringstream vertexShaderSource;
	vertexShaderSource << vsFile.rdbuf();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vsSource = vertexShaderSource.str().c_str();
	glShaderSource(vertexShader, 1, &vsSource, NULL);
	glCompileShader(vertexShader);
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE) {
		int maxLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		// TODO: Log
		glDeleteShader(vertexShader);
		throw -1;
	}
	
	// Compile pixel shader
	std::ifstream psFile(pixelShaderFile);
	std::stringstream pixelShaderSource;
	pixelShaderSource << psFile.rdbuf();
	unsigned int pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* psSource = pixelShaderSource.str().c_str();
	glShaderSource(pixelShader, 1, &psSource, NULL);
	glCompileShader(pixelShader);
	
	glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE) {
		int maxLength;
		glGetShaderiv(pixelShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(pixelShader, maxLength, &maxLength, &infoLog[0]);
		// TODO: Log
		glDeleteShader(pixelShader);
		throw -1;
	}
	
	// Compile geometry shader
	unsigned int geometryShader;
	if(geometryShaderFile != NULL) {
		std::ifstream gsFile(geometryShaderFile);
		std::stringstream geometryShaderSource;
		geometryShaderSource << gsFile.rdbuf();
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		const char* gsSource = geometryShaderSource.str().c_str();
		glShaderSource(geometryShader, 1, &gsSource, NULL);
		glCompileShader(geometryShader);
		
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE) {
			int maxLength;
			glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(geometryShader, maxLength, &maxLength, &infoLog[0]);
			// TODO: Log
			glDeleteShader(geometryShader);
			throw -1;
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
	
	glDetachShader(m_shaderProgram, vertexShader);
	glDetachShader(m_shaderProgram, pixelShader);
	glDetachShader(m_shaderProgram, vertexShader);
	if(geometryShaderFile != NULL) {
		glDetachShader(m_shaderProgram, geometryShader);
	}
}
