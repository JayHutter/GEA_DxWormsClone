#include "pch.h"
#include "PhysicsComp.h"

PhysicsComp::PhysicsComp(Vector2* _position)
{
	pos = _position;
}

void PhysicsComp::ApplyVelocity(float _gt)
{
	ControlSpeed();

	*pos += velocity * _gt;

	if (aerial)
	{
		ApplyGravity(aerial);
	}
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
		if (aerial)
		{
			//velocity = Vector2::Zero;
		}
		//Friction
		AddForce(Vector2(-(velocity.x/2), 0));
	}

	aerial = _falling;
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

void PhysicsComp::SetVelocityDir(Vector2 _velocity)
{
	_velocity.Normalize();
	float mag = Speed();
	velocity = mag * _velocity;
}

void PhysicsComp::SetSpeed(float _speed)
{
	velocity.Normalize();
	velocity *= _speed;
}


void PhysicsComp::AddForce(Vector2 _force)
{
	velocity += _force;
}

void PhysicsComp::MultiplyVelocity(Vector2 _force)
{
	velocity *= _force;
}

void PhysicsComp::MultiplyVelocity(float _power)
{
	velocity *= _power;
}

void PhysicsComp::ReactionForce(Vector2 _normal)
{
	float mag = Speed();
	velocity.Normalize();
	_normal.Normalize();

	//Dont add normal if moving away
	if ((velocity.x > 0 && _normal.x > 0) || (velocity.x < 0 && _normal.x < 0))
	{
		_normal.x = 0;
	}
	if ((velocity.y > 0 && _normal.y > 0) || (velocity.y < 0 && _normal.y < 0))
	{
		_normal.y = 0;
	}

	AddForce(_normal);
	velocity *= mag;
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


void PhysicsComp::ControlSpeed()
{
	if (false)
	{
		return;
	}

	if (velocity.x < min_speed && velocity.x > -min_speed)
	{
		velocity.x = 0;
	}
	if (velocity.y < min_speed && velocity.y > -min_speed)
	{
		velocity.y = 0;
	}

	if (velocity.x > max_speed || velocity.x < -max_speed)
	{
		velocity.x = max_speed;
	}
	if (velocity.y > max_speed || velocity.y < -max_speed)
	{
		velocity.y = max_speed;
	}
}

float PhysicsComp::Speed()
{
	return sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
}