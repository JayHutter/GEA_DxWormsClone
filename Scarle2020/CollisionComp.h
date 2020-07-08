#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <vector>

using namespace DirectX;
using namespace SimpleMath;

/*
Objects collision detection
*/
struct CollisionBox
{
	Vector2 pos_a = Vector2::Zero;
	Vector2 pos_b = Vector2::Zero;
};

/*
Collision box that damages hurt boxes (can add knockback)
*/
struct HitBox : public CollisionBox
{
	float damage = 0;
	Vector2 knockback = Vector2::Zero;
};

class CollisionComp
{
public:
	CollisionComp() = default;
	~CollisionComp() = default;

	void AddHurtbox(Vector2 _pos_a, Vector2 _pos_b);
	void AddHitBox(Vector2 _pos_a, Vector2 _pos_b, float _dmg, Vector2 _kb);

private:
	std::vector<CollisionBox> hurt_boxes;
	std::vector<HitBox> hit_boxes;
};

