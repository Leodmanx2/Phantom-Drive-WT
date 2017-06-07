#include "RenderModel.hpp"

RenderModel::RenderModel(const std::string& modelName) : name(modelName) {
	try {
		m_diffuseMap  = loadTexture(MODEL_DIR + name + "/diffuse.dds");
		m_specularMap = loadTexture(MODEL_DIR + name + "/specular.dds");
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, exception.what());
		throw std::runtime_error("Could not load model textures");
	}

	try {
		loadGeometry();
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, exception.what());
		throw std::runtime_error("Could not load model geometry");
	}
}

void RenderModel::loadGeometry() {
	// Prepare buffer data
	VertexList         vertices;
	IndexList          indices;
	const std::string  modelFilename = MODEL_DIR + name + "/model.mdl";
	const std::string  buffer        = readFile(modelFilename);
	std::istringstream fileStream(buffer, std::istringstream::binary);
	PMDL::File         fileData = PMDL::File::parse(fileStream);

	// Load model vertices into local buffer
	for(PMDL::Vertex fileVert : fileData.body.vertices) {
		Vertex vert;
		vert.position =
		  glm::vec3(fileVert.position.x, fileVert.position.y, fileVert.position.z);
		vert.normal =
		  glm::vec3(fileVert.normal.x, fileVert.normal.y, fileVert.normal.z);
		vert.texCoord = glm::vec2(fileVert.texCoord.u, fileVert.texCoord.v);
		vertices.push_back(vert);
	}

	// Load model indices into local buffer
	for(PMDL::Index fileInd : fileData.body.indices) {
		indices.push_back(fileInd);
	}
	m_elementCount = indices.size();

	// Send OpenGL our data
	try {
		fillBuffers(vertices, indices);
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, exception.what());
		std::stringstream message;
		message << "RenderModel3D (" << this
		        << "): Could not commit data to OpenGL";
		std::string messageString = message.str();
		throw std::runtime_error(messageString);
	}

	vaoSetup();
}

RenderModel::~RenderModel() {
	// Release GPU resources
	gl::glDeleteVertexArrays(1, &m_vertexArray);
	gl::glDeleteBuffers(1, &m_vertexBuffer);
	gl::glDeleteBuffers(1, &m_indexBuffer);
	gl::glDeleteTextures(1, &m_diffuseMap);
	gl::glDeleteTextures(1, &m_specularMap);
}

void RenderModel::fillBuffers(VertexList& vertices, IndexList& indices) {
	// Vertex buffer
	gl::glGenBuffers(1, &m_vertexBuffer);
	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, m_vertexBuffer);
	gl::glBufferData(gl::GL_ARRAY_BUFFER,
	                 sizeof(Vertex) * vertices.size(),
	                 vertices.data(),
	                 gl::GL_STATIC_DRAW);
	//Index buffer
	gl::glGenBuffers(1, &m_indexBuffer);
	gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	gl::glBufferData(gl::GL_ELEMENT_ARRAY_BUFFER,
	                 sizeof(unsigned int) * indices.size(),
	                 indices.data(),
	                 gl::GL_STATIC_DRAW);
}

void RenderModel::vaoSetup() {
	gl::glGenVertexArrays(1, &m_vertexArray);
	gl::glBindVertexArray(m_vertexArray);

	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, m_vertexBuffer);
	gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Save buffer layout descriptions to the VAO
	const int positionAttribPosition = 0;
	gl::glVertexAttribPointer(
	  positionAttribPosition,
	  3,
	  gl::GL_FLOAT,
	  gl::GL_FALSE,
	  sizeof(Vertex),
	  reinterpret_cast<void*>(offsetof(Vertex, position)));
	gl::glEnableVertexAttribArray(positionAttribPosition);

	const int normalAttribPosition = 1;
	gl::glVertexAttribPointer(normalAttribPosition,
	                          3,
	                          gl::GL_FLOAT,
	                          gl::GL_FALSE,
	                          sizeof(Vertex),
	                          reinterpret_cast<void*>(offsetof(Vertex, normal)));
	gl::glEnableVertexAttribArray(normalAttribPosition);

	const int texCoordAttribPosition = 2;
	gl::glVertexAttribPointer(
	  texCoordAttribPosition,
	  2,
	  gl::GL_FLOAT,
	  gl::GL_FALSE,
	  sizeof(Vertex),
	  reinterpret_cast<void*>(offsetof(Vertex, texCoord)));
	gl::glEnableVertexAttribArray(texCoordAttribPosition);

	// Clean up
	gl::glBindVertexArray(0);
}

/**
 * Parses a texture file into an OpenGL texture object.
 *
 * @param [in] filename     Relative path to the texture file in the mounted asset directory
 *
 * @return OpenGL id referencing the texture object in GPU memory
 */
gl::GLuint RenderModel::loadTexture(const std::string& filename) {
	std::string buffer = readFile(filename);

	// Create a GLI texture from the data read in by PhysFS
	gli::texture          texture = gli::load(buffer.data(), buffer.size());
	gli::gl               gl;
	const gli::gl::format format = gl.translate(texture.format());
	gl::GLenum target = static_cast<gl::GLenum>(gl.translate(texture.target()));
	if(target != gl::GL_TEXTURE_2D) {
		throw std::runtime_error(
		  "Texture target is not GL_TEXTURE_2D/gli::TARGET_2D");
	}
	if(texture.empty()) {
		throw std::runtime_error(filename + std::string(" is not a valid texture"));
	}

	// Reserve memory on the GPU for texture and describe its layout
	gl::GLuint textureID;
	gl::glGenTextures(1, &textureID);

	gl::glBindTexture(target, textureID);

	gl::glTexParameteri(target, gl::GL_TEXTURE_BASE_LEVEL, 0);
	gl::glTexParameteri(target, gl::GL_TEXTURE_MAX_LEVEL, texture.levels() - 1);
	gl::glTexParameteri(target, gl::GL_TEXTURE_SWIZZLE_R, format.Swizzle[0]);
	gl::glTexParameteri(target, gl::GL_TEXTURE_SWIZZLE_G, format.Swizzle[1]);
	gl::glTexParameteri(target, gl::GL_TEXTURE_SWIZZLE_B, format.Swizzle[2]);
	gl::glTexParameteri(target, gl::GL_TEXTURE_SWIZZLE_A, format.Swizzle[3]);

	gl::glTexStorage2D(target,
	                   texture.levels(),
	                   static_cast<gl::GLenum>(format.Internal),
	                   texture.dimensions().x,
	                   texture.dimensions().y);

	// Write image data to GPU memory
	for(unsigned int layer = 0; layer < texture.layers(); ++layer) {
		for(unsigned int face = 0; face < texture.faces(); ++face) {
			for(unsigned int level = 0; level < texture.levels(); ++level) {
				if(gli::is_compressed(texture.format())) {
					glCompressedTexSubImage2D(target,
					                          level,
					                          0,
					                          0,
					                          texture.dimensions().x,
					                          texture.dimensions().y,
					                          static_cast<gl::GLenum>(format.Internal),
					                          texture.size(level),
					                          texture.data(layer, face, level));
				} else {
					glTexSubImage2D(target,
					                level,
					                0,
					                0,
					                texture.dimensions().x,
					                texture.dimensions().y,
					                static_cast<gl::GLenum>(format.External),
					                static_cast<gl::GLenum>(format.Type),
					                texture.data(layer, face, level));
				}
			}
		}
	}

	return textureID;
}

/**
 * Draws the model to the active buffer
 *
 * @param [in] shader  The shader that will be used to draw the object
 */
void RenderModel::draw(Shader& shader) {
	shader.setDiffuseMap(m_diffuseMap);

	shader.setSpecularMap(m_specularMap);

	gl::glBindVertexArray(m_vertexArray);

	gl::glDrawElements(gl::GL_TRIANGLES, m_elementCount, gl::GL_UNSIGNED_INT, 0);

	gl::glBindVertexArray(0);
}
