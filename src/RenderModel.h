#ifndef RENDERMODEL_H
#define RENDERMODEL_H

class RenderModel {
	private:
		// GPU Resources
		unsigned int    m_vertexBuffer;
		unsigned int    m_indexBuffer;
		unsigned int    m_normalBuffer;
		unsigned int    m_textureCoordBuffer;
		
		unsigned int    m_shaderProgram;

	public:
		// Geometry shader is optional
		RenderModel(const char* modelFile, 
		            const char* vertexShaderFile, 
		            const char* pixelShaderFile, 
		            const char* geometryShaderFile = NULL);
								
		~RenderModel();
		
		virtual void draw();
};

#endif 