#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "Logger.h"
#include <physfs.h>
#include <limits>
#include <gli/gli.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Shader.h"

class RenderModel {
	private:
		static int     m_instanceCount;
	
	protected:
		RenderModel();
		explicit RenderModel(RenderModel& original);
	
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texCoord;
		};
		using VertexList = std::vector<Vertex>;
		using IndexList = std::vector<unsigned int>;
	
		// GPU Resources
		unsigned int   m_texture;
		unsigned int   m_normalMap;
		unsigned int   m_deltaMap;
		
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
		virtual ~RenderModel();
		
		virtual void draw(Shader& shader) = 0;
};

#endif
