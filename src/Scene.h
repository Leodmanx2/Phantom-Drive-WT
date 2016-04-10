#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"

#include "DummyActor.h"

class Scene {
	private:
		PhysicsSimulator* m_physicsSimulator;
		
		Shader* m_activeShader;
		
		Camera* m_activeCamera;
	
		//Actor* m_player;
		//Actor* m_player2;
		Actor* m_player3;
		
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
