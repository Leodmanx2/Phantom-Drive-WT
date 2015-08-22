#ifndef PHYSICSMODEL_H
#define PHYSICSMODEL_H

class PhysicsModel {
	public: enum CollisionShape{SPHERE, BOX, CYLINDER, CAPSULE, CONE, MESH};
	
	private:
		float m_mass;
		CollisionShape m_collisionShape;
	
	public:
		PhysicsModel();
		~PhysicsModel();
};

#endif
