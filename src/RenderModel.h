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

class RenderModel {
	protected:
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texCoord;
		};
	
		// GPU Resources
		unsigned int   m_vertexArray;
		
		unsigned int   m_vertexBuffer;
		unsigned int   m_indexBuffer;
		
		unsigned int   m_texture;
		unsigned int   m_sampler;
		
		unsigned int   m_shaderProgram;
		
		unsigned int   m_modelUniform;
		unsigned int   m_viewUniform;
		unsigned int   m_projectionUniform;
		unsigned int   m_textureUniform;
		
		unsigned int   loadTextureToGPU(const char* filename, 
		                                int* baseWidth, 
		                                int* baseHeight);

	public:
		virtual ~RenderModel();
		
		virtual void draw(float* modelMatrix, 
		                  float* viewMatrix, 
		                  float* projectionMatrix) = 0;
		
		void loadShaders(const char* vertexShaderFilename, 
		                 const char* pixelShaderFilename, 
		                 const char* geometryShaderFilename = nullptr);
};

#endif
