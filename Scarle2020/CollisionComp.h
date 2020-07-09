#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <vector>
#include <RenderTarget.h>

using namespace DirectX;
using namespace SimpleMath;

class CollisionComp
{
public:
	CollisionComp(float _width, float _height);
	~CollisionComp() = default;

	bool TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 pos);

private:
	RECT hitbox;

};

