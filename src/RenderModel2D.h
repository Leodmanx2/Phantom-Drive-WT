// The two-dimensional variant of the RenderModel class is a sprite

#ifndef RENDERMODEL2D_H
#define RENDERMODEL2D_H

#include "RenderModel.h"
#include <glm/gtc/type_ptr.hpp>

class RenderModel2D : public RenderModel {
	public:
		// Geometry shader is optional
		explicit RenderModel2D(const std::string& spriteFilename);
								
		~RenderModel2D();
		
		void draw(Shader& shader) override;
};

#endif
