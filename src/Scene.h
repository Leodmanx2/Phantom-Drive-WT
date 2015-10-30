#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"

#include "DummyActor.h"

class Scene {
	private:
		PhysicsSimulator* m_physicsSimulator;
	
		Actor* m_player;
		Actor* m_player2;
		Camera* m_activeCamera;
	
	public:
		Scene();
		Scene(const Scene& original);
		~Scene();
		
		void update();
		void simulate();
		void draw(float* projectionMatrix);
};

#endif
