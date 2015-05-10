#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <assimp/Importer.cpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class RenderModel {
	private:
		// GPU Resources
		unsigned int*   m_vertexBuffers;
		unsigned int*   m_indexBuffers;
		unsigned int*   m_normalBuffers;
		unsigned int*   m_textureCoordBuffers;
		
		unsigned int    m_shaderProgram;

	public:
		// Geometry shader is optional
		RenderModel(const char* modelFile, 
		            const char* vertexShaderFile, 
		            const char* pixelShaderFile, 
		            const char* geometryShaderFile = NULL);
								
		~RenderModel();
		
		void draw();
};

#endif
