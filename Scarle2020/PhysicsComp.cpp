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

//If falling apply gravity force
//Otherwise apply resistance
void PhysicsComp::ApplyGravity(bool _falling)
{
	if (_falling)
	{
		AddForce(Vector2(0, gravity));
	}
	else
	{
		AddForce(Vector2(-(velocity.x/10), -(velocity.y * 1.1F)));
	}
}