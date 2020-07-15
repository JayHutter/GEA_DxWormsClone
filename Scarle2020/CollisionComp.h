#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <vector>
#include <RenderTarget.h>

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
	CollisionComp(float _width, float _height);
	~CollisionComp() = default;

	bool TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pixel);
	bool TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pos, Side _side);
	bool TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pos, Side _vertical, Side _horizontal);
private:
	RECT hitbox;

};

