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
	void ApplyGravity();
private:
	float gravity = 9.0f;
	Vector2 velocity = Vector2::Zero;

	Vector2* pos = nullptr;
};

