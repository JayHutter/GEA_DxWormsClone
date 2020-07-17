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
}

//If falling apply gravity force
//Otherwise apply resistance
void PhysicsComp::ApplyGravity(bool _falling)
{
	if (_falling)
	{
		//AddForce(Vector2(0, gravity));
		//velocity.y = 10;
		//velocity.y = gravity;
		AddForce(Vector2(0, gravity));
	}
	else
	{
		if (aerial)
		{
			velocity = Vector2::Zero;
		}
		//velocity.y = -gravity;		
		//Friction
		AddForce(Vector2(-(velocity.x/2), 0));
		//MultiplyVelocity(Vector2(-0.7f, 1));
	}

	aerial = _falling;

	//velocity.y = 50 * _falling;

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
	float mag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
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
	_normal.Normalize();
	float mag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
	_normal *= gravity;

	AddForce(_normal);
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
	float mag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
	if (mag < min_speed && mag > -min_speed)
	{
		velocity = Vector2::Zero;
	}
	else if (mag > max_speed || mag < -max_speed)
	{
		velocity.Normalize();
		velocity *= max_speed;
	}
}
