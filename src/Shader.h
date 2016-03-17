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
		
		int            m_diffuseUniform;
		int            m_specularUniform;
		int            m_shineUniform;
		int            m_ambienceUniform;
		
		void           loadShaders(const std::string& vertexShaderFilename, 
		                           const std::string& pixelShaderFilename, 
		                           const std::string* geometryShaderFilename);
		
		unsigned int   compileShader(const std::string& filename, GLenum type);
		
		unsigned int   linkShaders(unsigned int vertexShader, 
		                           unsigned int pixelShader, 
		                           unsigned int geometryShader);
		
		unsigned int   linkShaders(unsigned int vertexShader, 
		                           unsigned int pixelShader);
	
	public:
		Shader(const std::string& vertexShaderFilename, 
	         const std::string& pixelShaderFilename, 
	         const std::string* geometryShaderFilename = nullptr);
					 
		virtual ~Shader();
		
		void bind();
		void unbind();
		bool isBound();
		
		void setModelMatrix(glm::mat4 matrix);
		void setViewMatrix(glm::mat4 matrix);
		void setProjectionMatrix(glm::mat4 matrix);
		
		void updateNormalMatrix();
		
		void setMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
		
		void setEyePosition(glm::vec3 position);
		
		void setDiffuseMap(unsigned int id);
		void setSpecularMap(unsigned int id);
		
		void setAmbience(float ambience);
};

#endif
