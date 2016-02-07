#include "RenderModel3D.h"

#define LOG_GL
#include "glerr.h"

RenderModel3D::RenderModel3D(const std::string& modelFilename)
{
    // Load sprite
	// TODO: We'll want to refactor a good deal of our file/texture laoding
	int baseWidth, baseHeight;
	try {m_texture = loadTextureToGPU("testWood.dds", &baseWidth, &baseHeight);}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not load RenderModel2D sprite");
	}
    
	// Prepare buffer data
	VertexList vertices;
	IndexList indices;
	
	loadOBJ(modelFilename, vertices, indices);
	
	// Send OpenGL our data
	try {
		fillBuffers(vertices, indices);
		glLogErr("Uploading buffer data");
	}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		std::stringstream message;
		message << "RenderModel3D (" << this << "): Could not commit data to OpenGL";
		std::string messageString = message.str();
		throw std::runtime_error(messageString);
	}
	
	vaoSetup();
}

RenderModel3D::~RenderModel3D() {
	
}

/**
 * Draws the model to the active buffer
 *
 * @param [in] shader  The shader that will be used to draw the object
 */
void RenderModel3D::draw(Shader& shader) {
	shader.setDiffuseMap(m_texture);
	glLogErr("Activating texture (3D)");
	
	glBindVertexArray(m_vertexArray);
	glLogErr("Binding VAO (3D)");
	
	glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_INT, 0);
	glLogErr("Drawing for real (3D)");
	
	glBindVertexArray(0);
	glLogErr("Un-binding VAO (3D)");
}

/**
 * This function is only for development purposes. It makes no 
 * use of the virtual file system provided by PhysicsFS.
 *
 * @param [in] filename   Model file name -- assumed to exist in 'ass' directory
 * @param [out] vertices  Reference to a std::vector<Vetrex> to be filled
 * @param [out] indices   Reference to a std::vector to be filled
 */
void RenderModel3D::loadOBJ(const std::string& filename, 
                            VertexList& vertices, 
                            IndexList& indices) {
	
	g_logger->write(Logger::DEBUG, "Starting model load");
	
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	
	std::string file = std::string("ass/") + filename;
	
	std::string err;
	bool ret = tinyobj::LoadObj(shapes, materials, err, file.c_str(), "ass/");
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
    const size_t texcoordCount = shapes[0].mesh.texcoords.size();
	std::stringstream debugVertexCount;
	std::stringstream debugIndexCount;
	std::stringstream debugNormalCount;
    std::stringstream debugTexcoordCount;
	debugVertexCount << "# of vertices: " << vertexCount;
	debugIndexCount << "# of indices: " << indexCount;
	debugNormalCount << "# of normals: " << normalCount;
    debugTexcoordCount << "# of texcoords: " << texcoordCount;
	g_logger->write(Logger::DEBUG, debugVertexCount.str());
	g_logger->write(Logger::DEBUG, debugIndexCount.str());
    g_logger->write(Logger::DEBUG, debugNormalCount.str());
	g_logger->write(Logger::DEBUG, debugTexcoordCount.str());
	
	m_elementCount = indexCount;

	g_logger->write(Logger::DEBUG, "Validating model");
	assert(indexCount % 3 == 0);
	assert(vertexCount == normalCount);
    assert(texcoordCount == vertexCount/3*2);
	
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
		
		vertex.texCoord = glm::vec2(shapes[0].mesh.texcoords[2 * i + 0],
		                            shapes[0].mesh.texcoords[2 * i + 1]);
		
		vertices.push_back(vertex);
	}
	
	g_logger->write(Logger::DEBUG, "Writing indices");
	
	for (size_t i = 0; i < indexCount; ++i) {
		indices.push_back(shapes[0].mesh.indices[i]);
	}
}