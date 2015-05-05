#include "RenderModel.h"

RenderModel::RenderModel(const char* modelFile, 
                         const char* vertexShaderFile, 
                         const char* pixelShaderFile, 
                         const char* geometryShaderFile)
{
	// Load 3D model
	Assimp::Importer importer;
	
	// -- Select properties that Assimp will discard
	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, 
	                            aiComponent_ANIMATIONS | 
	                            aiComponent_BONEWEIGHTS
															aiComponent_CAMERAS
															aiComponent_TANGENTS_AND_BITANGENTS);
	// -- Import the model
	// TODO: Direct to appropriate asset directory, or use an interface to asset 
	//       storage (recommended)
	const aiScene* scene = importer.ReadFile(std::string(modelFile), 
	                                         aiProcess_JoinIdenticalVertices |
	                                         aiProcess_Triangulate | 
	                                         aiProcess_RemoveComponent | 
	                                         aiProcess_GenNormals |
	                                         aiProcess_PreTransformVertices |
	                                         aiProcess_GenUVCoords |
	                                         aiProcess_TransformUVCoords |
	                                         aiProcess_OptimizeMeshes |
	                                         aiProcess_OptimizeGraph | 
	                                         aiProcess_Debone
	                                         )
																					 
	// Reserve buffer IDs for each of the meshes
	glGenBuffers(scene.mNumMeshes, m_vertexBuffers);
	glGenBuffers(scene.mNumMeshes, m_indexBuffers);
	glGenBuffers(scene.mNumMeshes, m_normalBuffers);
	glGenBuffers(scene.mNumMeshes, m_textureCoordBuffers);

	// Prepare vertex buffers
	for(int i=0; i<scene.mNumMeshes; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[i]);
		// TODO: replace NULL below with mesh vertices
		glBufferData(GL_ARRAY_BUFFER, sizeof(NULL), NULL, GL_STATIC_DRAW); 
	}
	
	// Prepare index buffer
	for(int i=0; i<scene.mNumMeshes; ++i) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[i]);
		// TODO: replace NULL below with mesh indices
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(NULL), NULL, GL_STATIC_DRAW);
	}
	
	// Prepare normal buffer
	for(int i=0; i<scene.mNumMeshes; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffers[i]);
		// TODO: replace NULL below with mesh normals
		glBufferData(GL_ARRAY_BUFFER, sizeof(NULL), NULL, GL_STATIC_DRAW);
	}
	
	// Prepare texture coordinate (UV) buffer
	for(int i=0; i<scene.mNumMeshes; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordBuffers[i]);
		// TODO: replace NULL below with mesh normals
		glBufferData(GL_ARRAY_BUFFER, sizeof(NULL), NULL, GL_STATIC_DRAW);
	}
	
	// TODO: Everything related to textures, animations, and lights
	
	// Compile shader program
	// TODO: ^
	// TODO: Review whether more than one shader per RenderModel is needed
}

RenderModel::~RenderModel() {
	// There should be an equal number of buffers of each type - one for every 
	// mesh in the Assimp scene
	unsigned int meshCount = sizeof(m_vertexBuffers)/sizeof(m_vertexBuffers[0]);
	
	// Release GPU resources
	glDeleteBuffers(meshCount, &m_vertexBuffers);
	glDeleteBuffers(meshCount, &m_indexBuffers);
	glDeleteBuffers(meshCount, &m_normalBuffers);
	glDeleteBuffers(meshCount, &m_textureCoordBuffers);

	glDeleteProgram(m_shaderPrograms);
}

RenderModel::draw() {
	// TODO: Draw all meshes
	
	// Code below copied from http://www.opengl-tutorial.org/
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   0                   // array buffer offset
	);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray(0);
}