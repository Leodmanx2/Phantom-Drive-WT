#include "Geometry.hpp"

#include "PMDL.hpp"

#include <algorithm>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <globjects/VertexAttributeBinding.h>
#include <plog/Log.h>
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
	LOG(plog::debug) << "constructing geometry";
	struct Vertex final {
		vec3 position;
		vec3 normal;
		vec2 texCoord;
	};

	// Prepare buffer data
	vector<Vertex>     vertices;
	vector<gl::GLuint> indices;
	ifstream           fileStream(name, ios::binary);
	PMDL::File         fileData = PMDL::File::parse(fileStream);

	// Load model vertices into local buffer
	for(PMDL::Vertex fileVert: fileData.body.vertices) {
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
	     back_inserter(indices));

	m_elementCount = indices.size();

	m_vertexBuffer->setData(vertices, GL_STATIC_DRAW);
	m_indexBuffer->setData(indices, GL_STATIC_DRAW);

	m_vertexArray->bind();
	m_vertexArray->bindElementBuffer(m_indexBuffer.get());

	auto positionBinding = m_vertexArray->binding(0);
	positionBinding->setAttribute(0);
	positionBinding->setBuffer(m_vertexBuffer.get(), 0, 32);
	positionBinding->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vertexArray->enable(0);

	auto normalBinding = m_vertexArray->binding(1);
	normalBinding->setAttribute(1);
	normalBinding->setBuffer(m_vertexBuffer.get(), 0, 32);
	normalBinding->setFormat(3, GL_FLOAT, GL_FALSE, 12);
	m_vertexArray->enable(1);

	auto uvBinding = m_vertexArray->binding(2);
	uvBinding->setAttribute(2);
	uvBinding->setBuffer(m_vertexBuffer.get(), 0, 32);
	uvBinding->setFormat(2, GL_FLOAT, GL_FALSE, 24);
	m_vertexArray->enable(2);
}

globjects::VertexArray& Geometry::vao() const { return *m_vertexArray; }

int Geometry::elements() const { return m_elementCount; }
