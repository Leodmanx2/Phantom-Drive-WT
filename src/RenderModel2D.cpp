#include "RenderModel2D.h"

RenderModel2D::RenderModel2D(const char* spriteFilename, 
	                           const char* vertexShaderFilename, 
	                           const char* pixelShaderFilename, 
	                           const char* geometryShaderFilename)
{
	// Compile shader program
	try {
		loadShaders(vertexShaderFilename, pixelShaderFilename, geometryShaderFilename);
	}
	catch(const std::runtime_error& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		
		std::stringstream message;
		message << "RenderModel2D (" << this << "): Could not load shaders";
		std::string messageString = message.str();
		throw std::runtime_error(messageString);
	}
	
	// Load sprite
	// TODO: We'll want to refactor a good deal of our file/texture laoding
	int baseWidth, baseHeight;
	try {m_texture = loadTextureToGPU(spriteFilename, &baseWidth, &baseHeight);}
	catch(const std::runtime_error& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not load RenderModel2D sprite");
	}
	
	// Reserve buffer IDs
	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_indexBuffer);
	
	// Prepare buffer
	Vertex* vertices = new Vertex[4];
	
	vertices[0].position = glm::vec3(-static_cast<float>(baseWidth)/2,  static_cast<float>(baseHeight)/2, 0.0f);
	vertices[0].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[0].texCoord = glm::vec2(0.0f, 1.0f);
	
	vertices[1].position = glm::vec3(static_cast<float>(baseWidth)/2, static_cast<float>(baseHeight)/2, 0.0f);
	vertices[1].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[1].texCoord = glm::vec2(1.0f, 1.0f);
	
	vertices[2].position = glm::vec3(static_cast<float>(baseWidth)/2, -static_cast<float>(baseHeight)/2, 0.0f);
	vertices[2].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[2].texCoord = glm::vec2(1.0f, 0.0f);
	
	vertices[3].position = glm::vec3(-static_cast<float>(baseWidth)/2, -static_cast<float>(baseHeight)/2, 0.0f);
	vertices[3].normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[3].texCoord = glm::vec2(0.0f, 0.0f);
	
	unsigned int indices[] = {
		2, 1, 0, 
		0, 3, 2
	};
	
	// Fill buffers on the GPU using the prepared arrays
	glBindVertexArray(m_vertexArray);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*4, vertices, GL_STATIC_DRAW);
	
	// NOTE: It is possible that the shader doesn't actually use these values, in 
	//       which case they will be optimized out by the compiler and their 
	//       location will be returned as -1 (or 0xFFFFFFFF if read as unsigned)
	unsigned int positionAttribute = glGetAttribLocation(m_shaderProgram, "position");
	bool posAttribEnabled = positionAttribute==0xFFFFFFFF ? false : true;
	if(posAttribEnabled) {
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		                      reinterpret_cast<void*>(offsetof(Vertex, position)));
		glEnableVertexAttribArray(positionAttribute);
	}

	unsigned int normalAttribute = glGetAttribLocation(m_shaderProgram, "normal");
	bool normAttribEnabled = normalAttribute==0xFFFFFFFF ? false : true;
	if(normAttribEnabled) {
		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		                      reinterpret_cast<void*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(normalAttribute);
	}

	unsigned int texCoordAttribute = glGetAttribLocation(m_shaderProgram, "texCoord");
	bool texCoordAttribEnabled = texCoordAttribute==0xFFFFFFFF ? false : true;
	if(texCoordAttribEnabled) {
		glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		                      reinterpret_cast<void*>(offsetof(Vertex, texCoord)));
		glEnableVertexAttribArray(texCoordAttribute);
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// TODO: Everything related to animations and lights
	
	// Clean up
	glBindVertexArray(0);
	delete[] vertices;
}

RenderModel2D::~RenderModel2D() {
	
}

void RenderModel2D::draw(float* modelMatrix, 
                         float* viewMatrix, 
                         float* projectionMatrix) {
	glUseProgram(m_shaderProgram);
	
	// TODO: Store uniform locations during initialization.
	//       Getting it each frame is expensive.
	unsigned int modelUniform = glGetUniformLocation(m_shaderProgram, "model");
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, modelMatrix);
	
	unsigned int viewUniform = glGetUniformLocation(m_shaderProgram, "view");
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, viewMatrix);
	
	unsigned int projectionUniform = glGetUniformLocation(m_shaderProgram, "projection");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, projectionMatrix);
	
	unsigned int textureUniform = glGetUniformLocation(m_shaderProgram, "textureSampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	
	glBindVertexArray(m_vertexArray);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}
