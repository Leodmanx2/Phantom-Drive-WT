#include "Geometry.hpp"

#include "pmdl.hpp"
#include "utility.hpp"
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/globjects.h>
#include <vector>

using namespace std;
using namespace globjects;
using namespace gl;
using namespace glm;

Geometry::Geometry(const string& name)
  : m_vertexArray(new VertexArray())
  , m_vertexBuffer(new Buffer())
  , m_indexBuffer(new Buffer())
  , m_elementCount(0) {
	struct Vertex final {
		vec3 position;
		vec3 normal;
		vec2 texCoord;
	};

	// Prepare buffer data
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	// FIXME: modelFilename is a magic string
	const string  modelFilename = "Models/"s + name + "/model.mdl"s;
	const string  buffer        = readFile(modelFilename);
	istringstream fileStream(buffer, istringstream::binary);
	PMDL::File    fileData = PMDL::File::parse(fileStream);

	// Load model vertices into local buffer
	for(PMDL::Vertex fileVert : fileData.body.vertices) {
		Vertex vert;
		vert.position =
		  vec3(fileVert.position.x, fileVert.position.y, fileVert.position.z);
		vert.normal = vec3(fileVert.normal.x, fileVert.normal.y, fileVert.normal.z);
		vert.texCoord = vec2(fileVert.texCoord.u, fileVert.texCoord.v);
		vertices.push_back(vert);
	}

	// Load model indices into local buffer
	copy(fileData.body.indices.begin(),
	     fileData.body.indices.end(),
	     indices.begin());

	m_elementCount = indices.size();

	m_vertexBuffer->setData(vertices, GL_ARRAY_BUFFER);
	m_indexBuffer->setData(indices, GL_ELEMENT_ARRAY_BUFFER);

	m_vertexArray->bind();
	m_vertexArray->bindElementBuffer(m_indexBuffer.get());

	auto positionBinding = m_vertexArray->binding(0);
	positionBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(Vertex));
	positionBinding->setFormat(3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	m_vertexArray->enable(0);

	auto normalBinding = m_vertexArray->binding(1);
	normalBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(Vertex));
	normalBinding->setFormat(3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	m_vertexArray->enable(1);

	auto uvBinding = m_vertexArray->binding(2);
	uvBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(Vertex));
	uvBinding->setFormat(2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
	m_vertexArray->enable(2);
}

globjects::VertexArray* Geometry::vao() { return m_vertexArray.get(); }
