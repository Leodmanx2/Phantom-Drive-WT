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
		void loadOBJ(const char* filename, 
		             VertexList& vertices, 
		             IndexList& indices);
	
	public:
		// Geometry shader is optional
		RenderModel3D(const char* modelFilename, 
		              const char* vertexShaderFilename, 
		              const char* pixelShaderFilename, 
		              const char* geometryShaderFilename = nullptr);
		
		~RenderModel3D();
		
		void draw(glm::mat4 modelMatrix, 
		          glm::mat4 viewMatrix, 
		          glm::mat4 projectionMatrix) override;
};

#endif
