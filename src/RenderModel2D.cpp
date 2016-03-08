#include "RenderModel2D.h"

#define LOG_GL
#include "glerr.h"

RenderModel2D::RenderModel2D(const std::string& spriteFilename)
{
	// Load sprite
	// TODO: We'll want to refactor a good deal of our file/texture laoding
	int baseWidth, baseHeight;
	try {m_diffuseMap = loadTextureToGPU(spriteFilename, &baseWidth, &baseHeight);}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not load RenderModel2D sprite");
	}
	
	// Prepare buffer data
	std::vector<Vertex> vertices(4);
	
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
	
	unsigned int indicesPlain[] = {
		2, 1, 0, 
		0, 3, 2
	};
	
	std::vector<unsigned int> indices(indicesPlain, indicesPlain + sizeof(indicesPlain)/sizeof(unsigned int));
	
	// Send OpenGL our data
	try {
		fillBuffers(vertices, indices);
		glLogErr("Uploading buffer data");
	}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		std::stringstream message;
		message << "RenderModel2D (" << this << "): Could not commit data to OpenGL";
		std::string messageString = message.str();
		throw std::runtime_error(messageString);
	}
	
	vaoSetup();
}

RenderModel2D::~RenderModel2D() {
	
}

/*
 * Draws the model to the active buffer
 *
 * @param [in] modelMatrix       The world transformation
 * @param [in] viewMatrix        Camera's view transformation
 * @param [in] projectionMatrix  World-to-screen transformation
 */
void RenderModel2D::draw(Shader& shader) {
	shader.setDiffuseMap(m_diffuseMap);
	glLogErr("Activating texture (2D)");
	
	glBindVertexArray(m_vertexArray);
	glLogErr("Binding VAO (2D)");
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glLogErr("Drawing for real (2D)");
	
	glBindVertexArray(0);
	glLogErr("Un-binding VAO (2D)");
}
