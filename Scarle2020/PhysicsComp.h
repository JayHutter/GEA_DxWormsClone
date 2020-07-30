#pragma once
#include <d3d11_1.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

class PhysicsComp
{
public:
	PhysicsComp(Vector2* _position, float _mu, float _bounce, bool _gravity);
	~PhysicsComp() = default;

	void ApplyVelocity(float _gt);
	void ApplyGravity(bool _falling);
	void ApplyGravity();
	void InAir(bool _aerial);

	void SetVelocity(Vector2 _force);
	void SetVelocityX(float _force);
	void SetVelocutyY(float _force);
	void SetVelocityDir(Vector2 _velocity);
	void SetSpeed(float _speed);
	void SetFrictionVal(float _mu);

	void StuckInGround(Vector2 _velocity);

	void AddForce(Vector2 _force);
	void MultiplyVelocity(Vector2 _force);
	void MultiplyVelocity(float _power);
	void ReactionForce(Vector2 _normal);

	bool MovingRight();
	bool MovingLeft();
	bool MovingDown();
	bool MovingUp();

	float AirTime() { return air_time; }

	Vector2 GetVel() { return velocity; }

private:
	void ControlSpeed();
	float Speed();
	void ApplyFriction(Vector2 _normal);

	bool applyGrav = true;
	float gravity = 10;
	Vector2 velocity = Vector2::Zero;
	bool aerial = true;

	Vector2* pos = nullptr;
	float max_speed = 1000;
	float min_speed = 10;
	float mu = 0.3f; //Coefficient of friction
	float bounce_multiplier = 0;

	float air_time = 0;
};

