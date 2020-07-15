#include "pch.h"
#include "CollisionComp.h"

CollisionComp::CollisionComp(float _width, float _height)
{
	hitbox.top = -(_height / 2);
	hitbox.bottom = _height / 2;
	hitbox.left = -(_width / 2);
	hitbox.right = _width / 2;
}

//Test if pixel has collided with visible stage
bool CollisionComp::TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pixel)
{
	//_render_target->Map(_context);
	//The bitmap is a few pixels higher than the actual render : Uknown cause
	const float collision_offset = 160;
	auto alpha = _render_target->GetPixel(_pixel.x, (-_pixel.y) + collision_offset, _GD)->A();
	
	if (alpha > 0.0F)
	{
		//_render_target->Unmap(_context);
		return true;
	}
	//_render_target->Unmap(_context);
	return false;
}

/*
//Test is side of object has collided with the stage
bool CollisionComp::TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pos, Side _side)
{
	Vector2 pixel = _pos;

	switch (_side)
	{
	case Side::Top:
		pixel.y += hitbox.top;
		break;
	case Side::Bottom:
		pixel.y += hitbox.bottom;
		break;
	case Side::Right:
		pixel.x += hitbox.right;
		break;
	case Side::Left:
		pixel.x += hitbox.left;
		break;
	}

	return TerrainCollision(_render_target, _context, _GD, pixel);
}
*/

bool CollisionComp::TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pos, Side _vert, Side _hori)
{
	switch (_vert)
	{
	case Side::Positive:
		_pos.y += hitbox.bottom;
		break;
	case Side::Negative:
		_pos.y += hitbox.top;
		break;
	}

	switch (_hori)
	{
	case Side::Positive:
		_pos.x += hitbox.right;
		break;
	case Side::Negative:
		_pos.x += hitbox.left;
	}

	return TerrainCollision(_render_target, _context, _GD, _pos);
}
