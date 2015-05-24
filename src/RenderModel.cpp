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
}

void RenderModel::loadShaders(const char* vertexShaderFile, 
                 const char* pixelShaderFile, 
                 const char* geometryShaderFile) {
	// TODO: Direct to appropriate directory, or use an interface to storage
	// Compile vertex shader
	std::ifstream vsFile(vertexShaderFile);
	std::stringstream vertexShaderSource;
	vertexShaderSource << vsFile.rdbuf();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vsSource = vertexShaderSource.str().c_str();
	glShaderSource(vertexShader, 1, &vsSource, NULL);
	
	// Compile pixel shader
	std::ifstream psFile(pixelShaderFile);
	std::stringstream pixelShaderSource;
	vertexShaderSource << psFile.rdbuf();
	unsigned int pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* psSource = pixelShaderSource.str().c_str();
	glShaderSource(pixelShader, 1, &psSource, NULL);
	
	// Compile geometry shader
	unsigned int geometryShader;
	if(geometryShaderFile != NULL) {
		std::ifstream gsFile(geometryShaderFile);
		std::stringstream geometryShaderSource;
		geometryShaderSource << gsFile.rdbuf();
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		const char* gsSource = geometryShaderSource.str().c_str();
		glShaderSource(geometryShader, 1, &gsSource, NULL);
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
