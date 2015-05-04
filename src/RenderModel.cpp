#include "RenderModel.h"

RenderModel::RenderModel(const char* modelFile, 
                         const char* vertexShaderFile, 
                         const char* pixelShaderFile, 
                         const char* geometryShaderFile)
{
	// Load 3D model
	
	// Prepare vertex buffer
	// Prepare vertex buffer with loaded data
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW); // TODO: NULL->vertices
	
	// Prepare index buffer
	
	// Prepare normal buffer
	
	// TODO: Everything texture-related
	
	// Compile shader program
}

RenderModel::~RenderModel() {
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
	glDeleteBuffers(1, &m_normalBuffer);
	glDeleteBuffers(1, &m_textureCoordBuffer);
}

RenderModel::draw() {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   0                   // array buffer offset
	);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray(0);
}