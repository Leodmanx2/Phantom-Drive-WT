#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"

#include "DummyActor.h"

class Scene {
	private:
		PhysicsSimulator* m_physicsSimulator;
		
		Shader* m_activeShader;
		
		Camera* m_activeCamera;
	
		// TODO: Remove specific actors, etc. These are for development purposes only.
		// Scene should not have details of specific scenes. Scenes must be built 
		// from external descriptions, just like Shader and RenderModel.
		Actor* m_player;
		
		float m_ambience; // How bright the scene is on average
	
	public:
		Scene();
		Scene(const Scene& original);
		~Scene();
		
		void update();
		void simulate();
		void draw(glm::mat4 projectionMatrix);
};

#endif
