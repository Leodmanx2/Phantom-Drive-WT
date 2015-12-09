#ifndef SHADER_H
#define SHADER_H

#include <stdexcept>
#include <sstream>
#include <GL/glew.h>
#include <gli/gli.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <physfs.h>
#include "Logger.h"

// TODO: Update render model construction to use shader object instead of making a shader themselves

class Shader {
	// Disable copying
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
	
	private:
		bool m_active;
	
		unsigned int   m_id;
		
		glm::mat4      m_modelMatrix;
		glm::mat4      m_viewMatrix;
		
		int            m_modelUniform;
		int            m_viewUniform;
		int            m_normalUniform;
		int            m_projectionUniform;
		
		int            m_eyePositionUniform;
		
		int            m_textureSamplerUniform;
		int            m_normalSamplerUniform;
		int            m_deltaSamplerUniform;
		
		void           loadShaders(const char* vertexShaderFilename, 
		                           const char* pixelShaderFilename, 
		                           const char* geometryShaderFilename);
		
		unsigned int   compileShader(const char* filename, GLenum type);
		
		unsigned int   linkShaders(unsigned int vertexShader, 
		                           unsigned int pixelShader, 
		                           unsigned int geometryShader);
		
		unsigned int   linkShaders(unsigned int vertexShader, 
		                           unsigned int pixelShader);
	
	public:
		Shader(const char* vertexShaderFilename, 
	         const char* pixelShaderFilename, 
	         const char* geometryShaderFilename = nullptr);
					 
		virtual ~Shader();
		
		void bind();
		bool isBound();
		
		void setModelMatrix(glm::mat4 matrix);
		void setViewMatrix(glm::mat4 matrix);
		void setProjectionMatrix(glm::mat4 matrix);
		
		void updateNormalMatrix();
		
		void setEyePosition(glm::vec3 position);
		
		void setTextureMap(unsigned int id);
		void setNormalMap(unsigned int id);
		void setDeltaMap(unsigned int id);
};

#endif
