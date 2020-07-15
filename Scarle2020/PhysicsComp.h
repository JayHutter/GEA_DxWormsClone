#pragma once
#include <d3d11_1.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

class PhysicsComp
{
public:
	PhysicsComp(Vector2* _position);
	~PhysicsComp() = default;

	void ApplyVelocity(float _gt);
	void AddForce(Vector2 _force);
	void SetVelocity(Vector2 _force);
	void SetVelocityX(float _force);
	void SetVelocutyY(float _force);
	void ResultantForce();
	void MultiplyVelocity(Vector2 _force);

	void ApplyGravity(bool _falling);
	
	bool MovingRight();
	bool MovingLeft();
	bool MovingDown();
	bool MovingUp();

private:
	bool applyGrav = false;
	float gravity = 10.0f;
	Vector2 velocity = Vector2::Zero;

	Vector2* pos = nullptr;
};

