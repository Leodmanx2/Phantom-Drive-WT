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

class RenderModel {
	protected:
		// GPU Resources
		unsigned int*   m_vertexBuffers;
		unsigned int*   m_indexBuffers;
		unsigned int*   m_normalBuffers;
		unsigned int*   m_textureCoordBuffers;
		unsigned int*   m_textures;
		
		unsigned int    m_shaderProgram;
		
		unsigned int    loadDDSTextureToGPU(const char* filename);

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
