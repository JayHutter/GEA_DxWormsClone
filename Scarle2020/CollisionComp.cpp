#include "pch.h"
#include "CollisionComp.h"

CollisionComp::CollisionComp(float _width, float _height)
{
	hitbox.top = -(_height / 2);
	hitbox.bottom = _height / 2;
	hitbox.left = -(_width / 2);
	hitbox.right = _width / 2;
}

//Test if pixel at bottom of hitbox has collided with visible stage
bool CollisionComp::TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 pos)
{
	_render_target->Map(_context);
	//The bitmap is a few pixels higher than the actual render : Uknown cause
	const float collision_offset = 125;
	auto alpha = _render_target->GetPixel(pos.x, (-pos.y) + hitbox.bottom + collision_offset, _GD)->A();

	if (alpha > 0.0F)
	{
		_render_target->Unmap(_context);
		return true;
	}
	_render_target->Unmap(_context);
	return false;
}