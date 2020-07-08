#include "PhysicsComp.h"

PhysicsComp::PhysicsComp(Vector2* _position)
{
	pos = _position;
}

void PhysicsComp::ApplyVelocity(float _gt)
{
	*pos += velocity * _gt;
}

void PhysicsComp::AddForce(Vector2 _force)
{
	velocity += _force;
}

void PhysicsComp::ApplyGravity(bool _falling)
{
	if (_falling)
	{
		AddForce(Vector2(0, gravity));
	}
	else
	{
		AddForce(Vector2(0, -(velocity.y)));
	}
}