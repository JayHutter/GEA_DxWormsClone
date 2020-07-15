#include "pch.h"
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

void PhysicsComp::SetVelocity(Vector2 _force)
{
	velocity = _force;
}

void PhysicsComp::SetVelocityX(float _force)
{
	velocity.x = _force;
}

void PhysicsComp::SetVelocutyY(float _force)
{
	velocity.y = _force;
}

//If falling apply gravity force
//Otherwise apply resistance
void PhysicsComp::ApplyGravity(bool _falling)
{
	if (_falling)
	{
		AddForce(Vector2(0, gravity));
		//velocity.y = 10;
	}
	else
	{
		//velocity.y = -gravity;
		//Friction
		//AddForce(Vector2(-(velocity.x/10), 0));
	}

	//velocity.y = 50 * _falling;
	
}

void PhysicsComp::ResultantForce()
{
	velocity *= -0.5f;
}

void PhysicsComp::MultiplyVelocity(Vector2 _force)
{
	velocity *= _force;
}

void PhysicsComp::ReactionForce(Vector2 _collider)
{
	Vector2 normal = XMVector2Orthogonal(_collider);
}

bool PhysicsComp::MovingRight()
{
	if (velocity.x > 0.5)
	{
		return true;
	}
	return false;
}

bool PhysicsComp::MovingLeft()
{
	if (velocity.x < 0.5)
	{
		return true;
	}
	return false;
}

bool PhysicsComp::MovingDown()
{
	if (velocity.y > 0.5)
	{
		return true;
	}
	return false;
}

bool PhysicsComp::MovingUp()
{
	if (velocity.y < 0.5)
	{
		return true;
	}
	return false;
}