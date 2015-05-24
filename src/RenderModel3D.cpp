#include "RenderModel3D.h"

RenderModel3D::RenderModel3D(const char* modelFile, 
                             const char* vertexShaderFile, 
                             const char* pixelShaderFile, 
                             const char* geometryShaderFile)
{
	// TODO: Handle case with no texCoords
	// TODO: Handle detail levels (related: 3d texcoords)
	
	Assimp::Importer importer;
	
	// TODO: Direct to appropriate directory, or use an interface to storage
	const aiScene* scene = importer.ReadFile(std::string(modelFile), 
	                                         aiProcessPreset_TargetRealtime_Fast);

	// Reserve buffer IDs for each of the meshes
	glGenBuffers(scene->mNumMeshes, m_vertexBuffers);
	glGenBuffers(scene->mNumMeshes, m_indexBuffers);
	glGenBuffers(scene->mNumMeshes, m_normalBuffers);
	glGenBuffers(scene->mNumMeshes, m_textureCoordBuffers);

	// Prepare buffers
	aiMesh* currMesh;
	aiFace currFace;
	for(unsigned int meshNum=0; meshNum<scene->mNumMeshes; ++meshNum) {
		currMesh = scene->mMeshes[meshNum];
		
		// Prepare arrays of vertices, normals, and texture coordinates to send 
		// to the GPU. There is a normal and texture coordinate for every vertex.
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texCoords;
		for(unsigned int vertNum=0; vertNum<currMesh->mNumVertices; ++vertNum) {
			vertices.push_back(currMesh->mVertices[vertNum].x);
			vertices.push_back(currMesh->mVertices[vertNum].y);
			vertices.push_back(currMesh->mVertices[vertNum].z);
			normals.push_back(currMesh->mNormals[vertNum].x);
			normals.push_back(currMesh->mNormals[vertNum].y);
			normals.push_back(currMesh->mNormals[vertNum].z);
			texCoords.push_back(currMesh->mTextureCoords[vertNum]->x);
			texCoords.push_back(currMesh->mTextureCoords[vertNum]->y);
			texCoords.push_back(currMesh->mTextureCoords[vertNum]->z);
			// Note: Assimp supports texture cubes. It is most likely the third 
			//       component (mTextureCoords.z) will be null, though.
		}
		
		// Prepare array of indices to send to GPU
		// Indices are stored in faces. There should be 3 indices per 
		// face, thanks to the aiProcess_Triangulate flag used in the 
		// post-processing preset we gave the Assimp importer.
		std::vector<unsigned int> indices;
		for(unsigned int faceNum=0; faceNum<currMesh->mNumFaces; ++faceNum) {
			currFace = currMesh->mFaces[faceNum];
			for(unsigned int indexNum=0; indexNum<currFace.mNumIndices; ++indexNum) {
				indices.push_back(currFace.mIndices[indexNum]);
			}
		}
		
		// Fill buffers on the GPU using the prepared arrays
		// - Vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[meshNum]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
		// - Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[meshNum]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
		// - Normal buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffers[meshNum]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals.data(), GL_STATIC_DRAW);
		// - Texture coordinate (UV) buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordBuffers[meshNum]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords.data(), GL_STATIC_DRAW);
	}
	
	// TODO: Everything related to textures, animations, and lights
	
	// Compile shader program
	// The geometry shader may be NULL
	loadShaders(vertexShaderFile, pixelShaderFile, geometryShaderFile);
	
	// TODO: Review whether more than one shader per RenderModel may be needed
}

RenderModel3D::~RenderModel3D() {
	// These values should all be equal, but it saves us some work to calculate 
	// them separately if we ever change the way RenderModels are initialized.
	unsigned int vertexCount = sizeof(m_vertexBuffers)/sizeof(m_vertexBuffers[0]);
	unsigned int indexCount = sizeof(m_indexBuffers)/sizeof(m_indexBuffers[0]);
	unsigned int normalCount = sizeof(m_normalBuffers)/sizeof(m_normalBuffers[0]);
	unsigned int texCoordCount = sizeof(m_textureCoordBuffers)/sizeof(m_textureCoordBuffers[0]);
	
	// Release GPU resources
	glDeleteBuffers(vertexCount, m_vertexBuffers);
	glDeleteBuffers(indexCount, m_indexBuffers);
	glDeleteBuffers(normalCount, m_normalBuffers);
	glDeleteBuffers(texCoordCount, m_textureCoordBuffers);

	glDeleteProgram(m_shaderProgram);
}

void RenderModel3D::draw() {
	// Draw all meshes
	// NOTE: Consider the use of a vertex array object
	// TODO: Add transformations (as OpenGL uniforms)
	glUseProgram(m_shaderProgram);
	
	for(unsigned int i=0; i<sizeof(m_vertexBuffers)/sizeof(m_vertexBuffers[0]); ++i) {
		unsigned int positionAttribute = glGetAttribLocation(m_shaderProgram, "position");
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[i]);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(positionAttribute);
		
		unsigned int normalAttribute = glGetAttribLocation(m_shaderProgram, "normal");
		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffers[i]);
		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(normalAttribute);
		
		unsigned int texCoordAttribute = glGetAttribLocation(m_shaderProgram, "texCoord");
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordBuffers[i]);
		glVertexAttribPointer(texCoordAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0); // 3D Coordinates
		glEnableVertexAttribArray(texCoordAttribute);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[i]);
		
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}
	
	glDisableVertexAttribArray(0);
}
