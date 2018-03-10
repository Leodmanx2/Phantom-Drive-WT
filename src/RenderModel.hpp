#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <cstddef>
#include <fstream>
#include <glbinding/gl/gl.h>
#include <gli/gli.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <limits>
#include <memory>
#include <pmdl.hpp>
#include <sstream>
#include <stdexcept>
#include <vector>

// Forward declarations ------------------------------------------------------
class Shader;
// ---------------------------------------------------------------------------

class RenderModel final {
	const std::string MODEL_DIR = "Models/";

	// Disable copying
	RenderModel(const RenderModel&) = delete;
	RenderModel& operator=(const RenderModel&) = delete;

	private:
	std::size_t m_elementCount;

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};
	using VertexList = std::vector<Vertex>;
	using IndexList  = std::vector<unsigned int>;

	// GPU Resources
	gl::GLuint m_diffuseMap;
	gl::GLuint m_specularMap;

	gl::GLuint m_vertexArray;

	gl::GLuint m_vertexBuffer;
	gl::GLuint m_indexBuffer;

	// Setup functions
	void fillBuffers(VertexList& vertices, IndexList& indices);
	void vaoSetup();
	void loadGeometry();

	static gl::GLuint loadTexture(const std::string& filename);

	public:
	explicit RenderModel(const std::string& modelFilename);
	~RenderModel();

	const std::string name;

	void draw(Shader& shader);
};

#endif
