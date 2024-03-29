#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <vector>
#include <RenderTarget.h>
#include <array>;

using namespace DirectX;
using namespace SimpleMath;

//enum class Side
//{
//	Top = 0,
//	Bottom,
//	Right,
//	Left,
//	Center
//};

enum class Side
{
	Center = 0,
	Positive,
	Negative
};

class CollisionComp
{
public:
	CollisionComp(float _width, float _height, Vector2 _pos);
	CollisionComp(float _width, float _height);
	~CollisionComp() = default;

	void SetHitbox(float _width, float _height, Vector2 _pos);
	void SetHitbox(float _width, float _height);

	bool TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pixel);
	//bool TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pos, Side _side);
	bool TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pos, Side _vertical, Side _horizontal);

	//void UpdateHitbox(Vector2 _pos);

	std::array<int, 4> TerrainCollsionV(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pos);
	Vector2 CalculateNormal(std::array<int, 4> _collisions);
	Vector2 CalculateNormal(RECT _other);

	bool Collided(RECT _other);

	void UpdateHitbox(Vector2 _pos);
	float HitboxWidth();
	float HitboxHeight();

	RECT Hitbox();

private:
	RECT hitbox;

	//The bitmap is a few pixels higher than the actual render : Uknown cause
	const float collision_offset = 160;
};

