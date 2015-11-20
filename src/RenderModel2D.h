// The two-dimensional variant of the RenderModel class is a sprite

#ifndef RENDERMODEL2D_H
#define RENDERMODEL2D_H

#include "RenderModel.h"
#include <glm/gtc/type_ptr.hpp>

class RenderModel2D : public RenderModel {
	public:
		// Geometry shader is optional
		RenderModel2D(const char* spriteFilename, 
		              const char* vertexShaderFilename, 
		              const char* pixelShaderFilename, 
		              const char* geometryShaderFilename = nullptr);
								
		~RenderModel2D();
		
		void draw(glm::mat4 modelMatrix, 
		          glm::mat4 viewMatrix, 
		          glm::mat4 projectionMatrix) override;
};

#endif
