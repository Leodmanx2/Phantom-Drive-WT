#ifndef SIMULATEDPHYSICSACTOR_H
#define SIMULATEDPHYSICSACTOR_H

#include "Actor.h"
#include "ActorMotionState.h"
#include "Logger.h"
#include "PhysicsSimulator.h"
#include "RenderModel.h"
#include <GL/glew.h>
#include <memory>
#include <stdexcept>

class SimulatedPhysicsActor : public Actor {
	protected:
	SimulatedPhysicsActor(PhysicsSimulator* simulator, btScalar mass);

	// Each child that inherets from this class must make its own,
	// preferrably static collision shape and override the default
	// by calling setCollisionShape() on m_body. Otherwise, the simulation
	// will use the default.
	// TODO: Consider improvements on this to make it clearer/easier
	static std::unique_ptr<btCollisionShape> s_defaultCollisionShape;
	PhysicsSimulator*                        m_simulator;
	btRigidBody*                             m_body;

	public:
	~SimulatedPhysicsActor();

	virtual void
	translate(float longitude, float latitude, float altitude) override;
	virtual void rotate(float roll, float pitch, float yaw) override;
};

#endif
