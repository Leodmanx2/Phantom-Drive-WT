#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"

#include "DummyActor.h"

class Scene {
	private:
		Actor* m_player;
		Camera* m_activeCamera;
	
	public:
		Scene();
		Scene(const Scene& original);
		~Scene();
		
		void update();
		void draw(float* projectionMatrix);
};

#endif
