#include "RenderModel.h"

RenderModel::~RenderModel() {
	unsigned int vertexCount = sizeof(m_vertexBuffers)/sizeof(m_vertexBuffers[0]);
	unsigned int indexCount = sizeof(m_indexBuffers)/sizeof(m_indexBuffers[0]);
	unsigned int normalCount = sizeof(m_normalBuffers)/sizeof(m_normalBuffers[0]);
	unsigned int texCoordCount = sizeof(m_textureCoordBuffers)/sizeof(m_textureCoordBuffers[0]);
	
	// Release GPU resources
	glDeleteBuffers(vertexCount, &m_vertexBuffers);
	glDeleteBuffers(indexCount, &m_indexBuffers);
	glDeleteBuffers(normalCount, &m_normalBuffers);
	glDeleteBuffers(texCoordCount, &m_textureCoordBuffers);

	glDeleteProgram(m_shaderPrograms);
}

void loadShaders(const char* vertexShaderFile, 
                 const char* pixelShaderFile, 
                 const char* geometryShaderFile) {
	// TODO: Direct to appropriate directory, or use an interface to storage
	// Compile vertex shader
	std::ifstream vsFile(vertexShaderFile);
	std::stringstream vertexShaderSource;
	vertexShaderSource << vsFile.rdbuf();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource.str().c_str(), NULL);
	
	// Compile pixel shader
	std::ifstream psFile(pixelShaderFile);
	std::stringstream pixelShaderSource;
	vertexShaderSource << psFile.rdbuf();
	unsigned int pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pixelShader, 1, &pixelShaderSource.str().c_str(), NULL);
	
	// Compile geometry shader
	if(geometryShaderFile != NULL) {
		std::ifstream gsFile(geometryShaderFile);
		std::stringstream geometryShaderSource;
		geometryShaderSource << gsFile.rdbuf();
		unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometryShaderSource.str().c_str(), NULL);
	}
	
	// Link shaders
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, pixelShader);
	if(geometryShaderFile != NULL) {
		glAttachShader(m_shaderProgram, geometryShader);
	}
	glLinkProgram(m_shaderProgram);
}
