#include "RenderModel.h"

RenderModel::RenderModel(const char* modelFile, 
                         const char* vertexShaderFile, 
                         const char* pixelShaderFile, 
                         const char* geometryShaderFile)
{
	// TODO: Direct to appropriate directory, or use an interface to storage

	// Reserve buffer IDs
	glGenBuffers(1, m_vertexBuffers);
	glGenBuffers(1, m_indexBuffers);
	glGenBuffers(1, m_normalBuffers);
	glGenBuffers(1, m_textureCoordBuffers);

	// Prepare buffers
	float vertices[] = {
		-0.5f,  0.5f, 0.0f, 
		 0.5f,  0.5f, 0.0f, 
		 0.5f, -0.5f, 0.0f, 
		-0.5f, -0.5f, 0.0f
	};
	
	// NOTE: Not sure if indices start from 0 or 1. Assuming 1 for now.
	unsigned int indices[] = {
		1, 2, 3, 
		1, 3, 4
	};
	
	// TODO: Read normal map into normal buffer
	
	float texCoords[] = {
		0.0f, 1.0f, 
		1.0f, 1.0f, 
		0.0f, 0.0f, 
		0.0f, 1.0f
	}
	
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
	// The geometry shader may be NULL
	loadShaders(vertexShaderFile, pixelShaderFile, geometryShaderFile);
}

RenderModel::~RenderModel() {
	
}

RenderModel::draw() {
	// NOTE: Consider the use of a vertex array object
	// TODO: Add transformations (as OpenGL uniforms)
	glUseProgram(m_shaderProgram);
	
	unsigned int positionAttribute = glGetAttribLocation(m_shaderProgram, "position");
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[i]);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);
	
	unsigned int normalAttribute = glGetAttribLocation(m_shaderProgram, "normal");
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffers[i]);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalAttribute);
	
	unsigned int texCoordAttribute = glGetAttribLocation(m_shaderProgram, "texCoord");
	glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordBuffers[i]);
	glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texCoordAttribute);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[0]);
	
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(0);
}
