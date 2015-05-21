// The two-dimensional variant of the RenderModel class is a sprite

#ifndef RENDERMODEL2D_H
#define RENDERMODEL2D_H

#include "RenderModel.h"

class RenderModel2D : RenderModel {
	public:
		// Geometry shader is optional
		RenderModel2D(const char* spriteFile, 
		              const char* vertexShaderFile, 
		              const char* pixelShaderFile, 
		              const char* geometryShaderFile = NULL);
								
		~RenderModel2D();
		
		void draw();
};

#endif
