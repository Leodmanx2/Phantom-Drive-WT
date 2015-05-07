#include "RenderModel.h"

RenderModel::RenderModel(const char* modelFile, 
                         const char* vertexShaderFile, 
                         const char* pixelShaderFile, 
                         const char* geometryShaderFile)
{
	Assimp::Importer importer;
	
	// TODO: Direct to appropriate asset directory, or use an interface to asset 
	//       storage (recommended)
	const aiScene* scene = importer.ReadFile(std::string(modelFile), 
	                                         aiProcessPreset_TargetRealtime_Fast)

	// Reserve buffer IDs for each of the meshes
	glGenBuffers(scene.mNumMeshes, m_vertexBuffers);
	glGenBuffers(scene.mNumMeshes, m_indexBuffers);
	glGenBuffers(scene.mNumMeshes, m_normalBuffers);
	glGenBuffers(scene.mNumMeshes, m_textureCoordBuffers);

	// Prepare buffers
	aiMesh* currMesh;
	aiFace currFace;
	for(int meshNum=0; meshNum<scene->mNumMeshes; ++meshNum) {
		currMesh = scene->mMeshes[meshNum]
		
		// Prepare arrays of vertices, normals, and texture coordinates to send 
		// to the GPU. There is a normal and texture coordinate for every vertex.
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texCoords;
		for(int vertNum=0; vertNum<currMesh->mNumVertices; ++vertNum) {
			vertices.push_back(currMesh->mVertices[vertNum].x);
			vertices.push_back(currMesh->mVertices[vertNum].y);
			vertices.push_back(currMesh->mVertices[vertNum].z);
			normals.push_back(currMesh->mNormals[vertNum].x);
			normals.push_back(currMesh->mNormals[vertNum].y);
			normals.push_back(currMesh->mNormals[vertNum].z);
			texCoords.push_back(currMesh->mTextureCoords.x);
			texCoords.push_back(currMesh->mTextureCoords.y);
			texCoords.push_back(currMesh->mTextureCoords.z);
			// Note: Assimp supports texture cubes. It is most likely the third 
			//       component (mTextureCoords.z) will be null, though.
		}
		
		// Prepare array of indices to send to GPU
		// Indices are stored in faces. There should be 3 indices per 
		// face, thanks to the aiProcess_Triangulate flag used in the 
		// post-processing preset we gave the Assimp importer.
		std::vector<unsigned int> indices;
		for(int faceNum=0; faceNum<currMesh->mNumFaces; ++faceNum) {
			currFace = currMesh->mFaces[faceNum];
			for(int indexNum=0; indexNum<currFace.mNumIndices; ++indexNum) {
				indices.push_back(currFace.mIndices[indexNum]);
			}
		}
		
		// Fill buffers on the GPU using the prepared arrays
		// - Vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
		// - Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
		// - Normal buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals.data(), GL_STATIC_DRAW);
		// - Texture coordinate (UV) buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords.data(), GL_STATIC_DRAW);
	}
	
	// TODO: Everything related to textures, animations, and lights
	
	// Compile shader program
	// TODO: ^
	// TODO: Review whether more than one shader per RenderModel is needed
}

RenderModel::~RenderModel() {
	// vertexCount, normalCount, and uvCount will probably be the same
	unsigned int vertexCount = sizeof(m_vertexBuffers)/sizeof(m_vertexBuffers[0]);
	unsigned int indexCount = sizeof(m_indexBuffers)/sizeof(m_indexBuffers[0]);
	unsigned int normalCount = sizeof(m_normalBuffers)/sizeof(m_normalBuffers[0]);
	unsigned int texCoordCount = sizeof(m_textureCoordBuffers)/sizeof(m_textureCoordBuffers[0]);
	
	// Release GPU resources
	glDeleteBuffers(vertexCount, &m_vertexBuffers);
	glDeleteBuffers(indexCount, &m_indexBuffers);
	glDeleteBuffers(normalCount, &m_normalBuffers);
	glDeleteBuffers(texCoordCount, &m_textureCoordBuffers);

	glDeleteProgram(m_shaderPrograms);
}

RenderModel::draw() {
	// TODO: Draw all meshes
	
	// This is old code that was used to draw a single triangle
	// It is kept here as reference for the above TODO.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray(0);
}
