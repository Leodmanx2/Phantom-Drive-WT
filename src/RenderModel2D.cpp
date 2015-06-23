#include "RenderModel2D.h"

RenderModel2D::RenderModel2D(const char* spriteFile, 
	                           const char* vertexShaderFile, 
	                           const char* pixelShaderFile, 
	                           const char* geometryShaderFile)
{
	// TODO: Use an interface to storage

	// Reserve buffer IDs
	m_vertexBuffers = new unsigned int[1];
	m_indexBuffers = new unsigned int[1];
	m_normalBuffers = new unsigned int[1];
	m_textureCoordBuffers = new unsigned int[1];
	glGenBuffers(1, m_vertexBuffers);
	glGenBuffers(1, m_indexBuffers);
	glGenBuffers(1, m_normalBuffers);
	glGenBuffers(1, m_textureCoordBuffers);

	// Prepare buffers
	// TODO: Quad must be sized to fit sprite
	float vertices[] = {
		-0.5f,  0.5f, 0.0f, 
		 0.5f,  0.5f, 0.0f, 
		 0.5f, -0.5f, 0.0f, 
		-0.5f, -0.5f, 0.0f
	};
	
	unsigned int indices[] = {
		2, 1, 0,  
		0, 3, 2
	};
	
	float normals[] = {
		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, 1.0f
	};
	
	float texCoords[] = {
		0.0f, 1.0f, 
		1.0f, 1.0f, 
		0.0f, 0.0f, 
		0.0f, 1.0f
	};
	
	// Fill buffers on the GPU using the prepared arrays
	// - Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// - Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// - Normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	// - Texture coordinate (UV) buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	
	// TODO: Everything related to textures, animations, and lights
	
	// Compile shader program
	loadShaders(vertexShaderFile, pixelShaderFile, geometryShaderFile);
}

RenderModel2D::~RenderModel2D() {
	
}

void RenderModel2D::draw(float* modelMatrix, 
                         float* viewMatrix, 
                         float* projectionMatrix) {
	// NOTE: Consider the use of a vertex array object
	glUseProgram(m_shaderProgram);
	
	unsigned int modelUniform = glGetUniformLocation(m_shaderProgram, "model");
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, modelMatrix);
	
	unsigned int viewUniform = glGetUniformLocation(m_shaderProgram, "view");
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, viewMatrix);
	
	unsigned int projectionUniform = glGetUniformLocation(m_shaderProgram, "projection");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, projectionMatrix);
	
	unsigned int positionAttribute = glGetAttribLocation(m_shaderProgram, "position");
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[0]);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);
	
	unsigned int normalAttribute = glGetAttribLocation(m_shaderProgram, "normal");
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffers[0]);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalAttribute);
	
	unsigned int texCoordAttribute = glGetAttribLocation(m_shaderProgram, "texCoord");
	glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordBuffers[0]);
	glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texCoordAttribute);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[0]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(normalAttribute);
	glDisableVertexAttribArray(texCoordAttribute);
}
