#ifndef PD_GEOMETRY_HPP
#define PD_GEOMETRY_HPP

#include <globjects/globjects.h>
#include <string>

class Geometry final {
	std::unique_ptr<globjects::VertexArray> m_vertexArray;
	std::unique_ptr<globjects::Buffer>      m_vertexBuffer;
	std::unique_ptr<globjects::Buffer>      m_indexBuffer;
	int                                     m_elementCount;

	public:
	explicit Geometry(const std::string& name);
	globjects::VertexArray& vao() const;
	int                     elements() const;
};

#endif
