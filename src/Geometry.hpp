#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <string>

class Geometry final {
	std::unique_ptr<globjects::VertexArray> m_vertexArray;
	std::unique_ptr<globjects::Buffer>      m_vertexBuffer;
	std::unique_ptr<globjects::Buffer>      m_indexBuffer;
	int                                     m_elementCount;

	public:
	explicit Geometry(const std::string& name);
	globjects::VertexArray* vao() const;
	int                     elements() const;
};

#endif
