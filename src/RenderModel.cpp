#include "RenderModel.h"

#define LOG_GL
#include "glerr.h"

int RenderModel::m_instanceCount = 0;

RenderModel::RenderModel(const std::string& modelName) {
	++m_instanceCount;

	// Load sprite
	// TODO: We'll want to refactor a good deal of our file/texture loading
	int baseWidth, baseHeight;
	try {
		m_diffuseMap =
		  loadTextureToGPU(modelName + "/diffuse.dds", &baseWidth, &baseHeight);
	} catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not load RenderModel3D diffuse map");
	}

	int baseWidth2, baseHeight2;
	try {
		m_specularMap =
		  loadTextureToGPU(modelName + "/specular.dds", &baseWidth2, &baseHeight2);
	} catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Could not load RenderModel3D specular map");
	}

	// Prepare buffer data
	VertexList vertices;
	IndexList  indices;
	//std::string modelFilename = modelName + "/model.mdl";
	// TODO: Load geometry

	// Send OpenGL our data
	try {
		fillBuffers(vertices, indices);
		glLogErr("Uploading buffer data");
	} catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		std::stringstream message;
		message << "RenderModel3D (" << this
		        << "): Could not commit data to OpenGL";
		std::string messageString = message.str();
		throw std::runtime_error(messageString);
	}

	vaoSetup();
}

RenderModel::RenderModel(const RenderModel&) { ++m_instanceCount; }

RenderModel::~RenderModel() {
	--m_instanceCount;
	if(m_instanceCount == 0) {
		// Release GPU resources
		glDeleteVertexArrays(1, &m_vertexArray);
		glDeleteBuffers(1, &m_vertexBuffer);
		glDeleteBuffers(1, &m_indexBuffer);
		glDeleteTextures(1, &m_diffuseMap);
		glDeleteTextures(1, &m_specularMap);
	}
}

void RenderModel::fillBuffers(VertexList& vertices, IndexList& indices) {
	// Vertex buffer
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(Vertex) * vertices.size(),
	             vertices.data(),
	             GL_STATIC_DRAW);
	//Index buffer
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             sizeof(unsigned int) * indices.size(),
	             indices.data(),
	             GL_STATIC_DRAW);
}

void RenderModel::vaoSetup() {
	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Save buffer layout descriptions to the VAO
	const int positionAttribPosition = 0;
	glVertexAttribPointer(positionAttribPosition,
	                      3,
	                      GL_FLOAT,
	                      GL_FALSE,
	                      sizeof(Vertex),
	                      reinterpret_cast<void*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(positionAttribPosition);

	const int normalAttribPosition = 1;
	glVertexAttribPointer(normalAttribPosition,
	                      3,
	                      GL_FLOAT,
	                      GL_FALSE,
	                      sizeof(Vertex),
	                      reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(normalAttribPosition);

	const int texCoordAttribPosition = 2;
	glVertexAttribPointer(texCoordAttribPosition,
	                      2,
	                      GL_FLOAT,
	                      GL_FALSE,
	                      sizeof(Vertex),
	                      reinterpret_cast<void*>(offsetof(Vertex, texCoord)));
	glEnableVertexAttribArray(texCoordAttribPosition);

	// TODO: Everything related to animations

	// Clean up
	glBindVertexArray(0);
}

/**
 * Parses a texture file into an OpenGL texture object.
 *
 * @param [in] filename     Relative path to the texture file in the mounted asset directory
 * @param [out] baseWidth   The width of the standard-detail image. Used as a hack to size render models.
 * @param [out] baseHeight  The height of the standard-detail image. Used as a hack to size render models.
 *
 * @return OpenGL id referencing the texture object in GPU memory
 */
unsigned int RenderModel::loadTextureToGPU(const std::string& filename,
                                           int*               baseWidth,
                                           int*               baseHeight) {
	// Use PhysFS to read texture file into memory
	if(filename.compare("") == 0 || !PHYSFS_exists(filename.c_str()))
		throw std::runtime_error(std::string("Could not find texture: ") +
		                         filename);

	PHYSFS_File* file = PHYSFS_openRead(filename.c_str());
	if(!file)
		throw std::runtime_error(std::string("Could not open texture: ") +
		                         filename);

	PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
	if(fileSize == -1)
		throw std::runtime_error(
		  std::string("Could not determine size of texture: ") + filename);

	char* buffer    = new char[fileSize];
	int   bytesRead = PHYSFS_read(file, buffer, 1, fileSize);
	PHYSFS_close(file);
	if(bytesRead < fileSize || bytesRead == -1) {
		delete[] buffer;
		g_logger->write(Logger::ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of texture: ") +
		                         filename);
	}

	// Create a GLI texture from the data read in by PhysFS
	gli::texture texture = gli::load(buffer, fileSize);
	delete[] buffer;
	gli::gl               gl;
	const gli::gl::format format = gl.translate(texture.format());
	GLenum                target = gl.translate(texture.target());
	if(target != GL_TEXTURE_2D) {
		throw std::runtime_error(
		  "Texture target is not GL_TEXTURE_2D/gli::TARGET_2D");
	}
	if(texture.empty()) {
		throw std::runtime_error(filename + std::string(" is not a valid texture"));
	}

	// We currently need to write these out in order to size
	// 2d render models. This should be written out in the
	// near future.
	*baseWidth  = texture.dimensions().x;
	*baseHeight = texture.dimensions().y;

	// Reserve memory on the GPU for texture and describe its layout
	unsigned int textureID;
	glGenTextures(1, &textureID);

	glBindTexture(target, textureID);

	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, texture.levels() - 1);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzle[0]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzle[1]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzle[2]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzle[3]);

	glTexStorage2D(target,
	               texture.levels(),
	               format.Internal,
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
					                          format.Internal,
					                          texture.size(level),
					                          texture.data(layer, face, level));
				} else {
					glTexSubImage2D(target,
					                level,
					                0,
					                0,
					                texture.dimensions().x,
					                texture.dimensions().y,
					                format.External,
					                format.Type,
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
	glLogErr("Activating diffuse texture (3D)");

	shader.setSpecularMap(m_specularMap);
	glLogErr("Activating specular texture (3D)");

	glBindVertexArray(m_vertexArray);
	glLogErr("Binding VAO (3D)");

	glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_INT, 0);
	glLogErr("Drawing for real (3D)");

	glBindVertexArray(0);
	glLogErr("Un-binding VAO (3D)");
}
