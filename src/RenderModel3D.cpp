#include "RenderModel3D.h"

RenderModel3D::RenderModel3D(const char* modelFilename, 
	                           const char* vertexShaderFilename, 
	                           const char* pixelShaderFilename, 
	                           const char* geometryShaderFilename)
{
	// Prepare buffer data
	VertexList vertices;
	IndexList indices;
	
	loadOBJ(modelFilename, vertices, indices);
	
	// Compile shader program
	try {
		loadShaders(vertexShaderFilename, pixelShaderFilename, geometryShaderFilename);
	}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		
		std::stringstream message;
		message << "RenderModel2D (" << this << "): Could not load shaders";
		std::string messageString = message.str();
		throw std::runtime_error(messageString);
	}
	
	// Send OpenGL our data and describe how to use it
	try {
		glSetup(m_shaderProgram, vertices, indices);
	}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		std::stringstream message;
		message << "RenderModel2D (" << this << "): Could not commit data to OpenGL";
		std::string messageString = message.str();
		throw std::runtime_error(messageString);
	}
}

RenderModel3D::~RenderModel3D() {
	
}

/**
 * Draws the model to the active buffer
 *
 * @param [in] modelMatrix       The world transformation
 * @param [in] viewMatrix        Camera's view transformation
 * @param [in] projectionMatrix  World-to-screen transformation
 */
void RenderModel3D::draw(glm::mat4 modelMatrix, 
                         glm::mat4 viewMatrix, 
                         glm::mat4 projectionMatrix) {
	glUseProgram(m_shaderProgram);
	
	glUniformMatrix4fv(m_modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	
	glBindVertexArray(m_vertexArray);
	
	glDrawElements(GL_TRIANGLES, 492, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}

/**
 * This function is only for development purposes. It makes no 
 * use of the virtual file system provided by PhysicsFS.
 *
 * @param [in] filename   Model file name -- assumed to exist in 'ass' directory
 * @param [out] vertices  Reference to a std::vector<Vetrex> to be filled
 * @param [out] indices   Reference to a std::vector to be filled
 */
void RenderModel3D::loadOBJ(const char* filename, 
                            VertexList& vertices, 
                            IndexList& indices) {
	
	g_logger->write(Logger::DEBUG, "Starting model load");
	
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	
	std::string file = std::string("ass/") + filename;
	
	std::string err;
	bool ret = tinyobj::LoadObj(shapes, materials, err, file.c_str());
	// We will not bother handling any errors, just crashing and burning
	if (!err.empty()) {
		g_logger->write(Logger::ERROR, err.c_str());
	}
	if (!ret) {
		exit(EXIT_FAILURE);
	}
	
	g_logger->write(Logger::DEBUG, "Obtaining counts");
	
	std::stringstream debugShapeCount;
	std::stringstream debugMaterialCount;
	debugShapeCount << "# of shapes: " << shapes.size();
	debugMaterialCount << "# of materials: " << materials.size();
	g_logger->write(Logger::DEBUG, debugShapeCount.str());
	g_logger->write(Logger::DEBUG, debugMaterialCount.str());
	
	// We're assuming there is only one shape
	const size_t vertexCount = shapes[0].mesh.positions.size();
	const size_t indexCount = shapes[0].mesh.indices.size();
	const size_t normalCount = shapes[0].mesh.normals.size();
	std::stringstream debugVertexCount;
	std::stringstream debugIndexCount;
	std::stringstream debugNormalCount;
	debugVertexCount << "# of vertices: " << vertexCount;
	debugIndexCount << "# of indices: " << indexCount;
	debugNormalCount << "# of normals: " << normalCount;
	g_logger->write(Logger::DEBUG, debugVertexCount.str());
	g_logger->write(Logger::DEBUG, debugIndexCount.str());
	g_logger->write(Logger::DEBUG, debugNormalCount.str());

	g_logger->write(Logger::DEBUG, "Validating model");
	assert(indexCount % 3 == 0);
	assert(vertexCount == normalCount);
	
	std::unique_ptr<glm::vec3[]> positions(new glm::vec3[vertexCount]);
	std::unique_ptr<glm::vec3[]> normals(new glm::vec3[normalCount]);
	
	g_logger->write(Logger::DEBUG, "Filling vertex structures");
	
	Vertex vertex;
	for (size_t i = 0; i < vertexCount / 3; ++i) {
		vertex.position = glm::vec3(shapes[0].mesh.positions[3 * i + 0],
		                            shapes[0].mesh.positions[3 * i + 1],
		                            shapes[0].mesh.positions[3 * i + 2]);
														 
		vertex.normal = glm::vec3(shapes[0].mesh.normals[3 * i + 0],
		                          shapes[0].mesh.normals[3 * i + 1],
		                          shapes[0].mesh.normals[3 * i + 2]);
		
		vertex.texCoord = glm::vec2(0, 0);
		
		vertices.push_back(vertex);
	}
	
	g_logger->write(Logger::DEBUG, "Writing indices");
	
	for (size_t i = 0; i < indexCount; ++i) {
		indices.push_back(shapes[0].mesh.indices[i]);
	}
}
