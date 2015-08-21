#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"

#include "DummyActor.h"

class Scene {
	private:
		Actor* m_actor;
		Camera* m_activeCamera;
	
	public:
		Scene(const char* sceneFile);
		~Scene();
		
		void update();
		void draw(float* projectionMatrix);
};

#endif
