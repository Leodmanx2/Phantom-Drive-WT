#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"

#include "DummyActor.h"
#include "DummyActor2.h"

class Scene {
	private:
		PhysicsSimulator* m_physicsSimulator;
	
		Actor* m_player;
		Actor* m_player2;
		Actor* m_player3;
		Camera* m_activeCamera;
		
		// OpenGL resources
		unsigned int m_eyePosUniform;
		unsigned int m_lightPos0;
	
	public:
		Scene();
		Scene(const Scene& original);
		~Scene();
		
		void update();
		void simulate();
		void draw(glm::mat4 projectionMatrix);
};

#endif
