#ifndef RENDERMODEL3D_H
#define RENDERMODEL3D_H

#include "RenderModel.h"
#include <glm/gtc/type_ptr.hpp>

// We're only using tiny_obj_loader for development purposes. 
// We'll require something supporting animations when we get to that.
// Will eventually implement own model format.
#include "tiny_obj_loader.h"
#include <iostream>
#include <memory>

class RenderModel3D : public RenderModel {
	private:
		size_t m_elementCount;
	
		// TODO: Replace with other model-loading code when required
		void loadOBJ(const std::string& filename, 
		             VertexList& vertices, 
		             IndexList& indices);
	
	public:
		// Geometry shader is optional
		explicit RenderModel3D(const std::string& modelFilename);
		
		~RenderModel3D();
		
		void draw(Shader& shader) override;
};

#endif
