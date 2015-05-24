#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <GL/glew.h>
#include <fstream>
#include <sstream>

class RenderModel {
	protected:
		// GPU Resources
		unsigned int*   m_vertexBuffers;
		unsigned int*   m_indexBuffers;
		unsigned int*   m_normalBuffers;
		unsigned int*   m_textureCoordBuffers;
		
		unsigned int    m_shaderProgram;

	public:
		virtual ~RenderModel();
		
		virtual void draw() = 0;
		
		void loadShaders(const char* vertexShaderFile, 
                     const char* pixelShaderFile, 
                     const char* geometryShaderFile = NULL);
};

#endif
