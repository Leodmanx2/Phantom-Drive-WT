#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <physfs.h>
#include <limits>
#include <gli/gli.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include "Logger.h"
#include "Shader.h"

class RenderModel {
	private:
		static int     m_instanceCount;
		
		size_t m_elementCount;
	
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texCoord;
		};
		using VertexList = std::vector<Vertex>;
		using IndexList = std::vector<unsigned int>;
	
		// TODO: Replace with other model-loading code when required
		void loadOBJ(const std::string& filename, 
		             VertexList& vertices, 
		             IndexList& indices);
	
	protected:
		RenderModel();
		explicit RenderModel(const RenderModel&);
	
		// GPU Resources
		unsigned int   m_diffuseMap;
		unsigned int   m_specularMap;
		
		unsigned int   m_vertexArray;
		
		unsigned int   m_vertexBuffer;
		unsigned int   m_indexBuffer;
		
		// Setup functions
		void           fillBuffers(VertexList& vertices, IndexList& indices);
		
		void           vaoSetup();
		
		unsigned int   loadTextureToGPU(const std::string& filename, 
		                                int* baseWidth, 
		                                int* baseHeight);

	public:
		// Geometry shader is optional
		explicit RenderModel(const std::string& modelFilename);
		
		virtual ~RenderModel();
		
		void draw(Shader& shader);
};

#endif
