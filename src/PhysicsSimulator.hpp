#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <btBulletDynamicsCommon.h>
#include <memory>

class PhysicsSimulator final {
	private:
	static std::unique_ptr<btBroadphaseInterface> s_broadphase;
	static std::unique_ptr<btDefaultCollisionConfiguration>
	                                                            s_collisionConfiguration;
	static std::unique_ptr<btCollisionDispatcher>               s_dispatcher;
	static std::unique_ptr<btSequentialImpulseConstraintSolver> s_solver;

	btDiscreteDynamicsWorld* m_world;

	public:
	PhysicsSimulator();
	PhysicsSimulator(PhysicsSimulator& original);
	~PhysicsSimulator();

	void addRigidBody(btRigidBody* body);
	void removeRigidBody(btRigidBody* body);
	void stepSimulation();
};

#endif
