#include <Actor.h>

Actor::Actor() {
	// TODO: Move into RenderModel, make workable for alternate shader pipelines
	
	// Load 3D model
	
	// Prepare vertex buffer with loaded data
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
	
	// Prepare index buffer with loaded data
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);
	
	/*
	// Prepare vertex shader
	char* vsSource = filetobuf("sampleVS.glsl");
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertexShader, 1, &vsSource, 0);
	free(vsSource);
	glCompileShader(m_vertexShader);
	
	// Prepare pixel shader
	char* psSource = filetobuf("samplePS.glsl");
	m_pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_pixelShader, 1, &psSource, 0);
	free(psSource);
	glCompileShader(m_pixelShader);
	
	// Link compiled shaders
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_pixelShader);
	glLinkProgram(m_shaderProgram);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_pixelShader);
	
	// TODO: Localize unlinked shaders
	*/
}

Actor::~Actor() {
	delete m_vertices;
	delete m_indices;
	delete m_normals;
	
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
	
	// TODO: Deal with shader resources
}

void translate(float longitude, float latitude, float altitude) {
	
}

void rotate(float roll, float pitch, float yaw) {
	
}

void update() {
	
}

void draw() {
	// Prepare resources on GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	//glUseProgram(m_shaderProgram);
	
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	// Draw using the prepared resources
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0); // TODO: Work with vec3
	
	// Post-draw cleanup
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
