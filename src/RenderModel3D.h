// RenderModel3D is severely out-of-date

#ifndef RENDERMODEL3D_H
#define RENDERMODEL3D_H

#include "RenderModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class RenderModel3D : public RenderModel {
	public:
		// Geometry shader is optional
		RenderModel3D(const char* modelFile, 
		              const char* vertexShaderFile, 
		              const char* pixelShaderFile, 
		              const char* geometryShaderFile = NULL);
								
		~RenderModel3D();
		
		void draw();
};

#endif
