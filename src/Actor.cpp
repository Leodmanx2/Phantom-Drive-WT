#include "Actor.h"

Actor::Actor() {
	// TODO: Move into RenderModel, make workable for alternate shader pipelines
	
	// Load 3D model
	float vertices[] = {
		 -1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};
	
	// Prepare vertex buffer with loaded data
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

Actor::~Actor() {
	glDeleteBuffers(1, &m_vertexBuffer);
}

void Actor::translate(float longitude, float latitude, float altitude) {
	
}

void Actor::rotate(float roll, float pitch, float yaw) {
	
}

void Actor::update() {
	
}

void Actor::draw() {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(
		 0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		 3,                  // size
		 GL_FLOAT,           // type
		 GL_FALSE,           // normalized?
		 0,                  // stride
		 (void*)0            // array buffer offset
	);
	 
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	 
	glDisableVertexAttribArray(0);
}
