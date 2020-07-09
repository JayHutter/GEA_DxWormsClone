#include "CollisionComp.h"

CollisionComp::CollisionComp(float _width, float _height)
{
	hitbox.top = -(_height / 2);
	hitbox.bottom = _height / 2;
	hitbox.left = -(_width / 2);
	hitbox.right = _width / 2;
}

bool CollisionComp::TerrainCollision(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 pos)
{
	/*
	for (int x = (pos.x + hitbox.left); x++; x < (pos.x + hitbox.right))
	{
		for (int y = (pos.y + hitbox.top); y++; y < (pos.y + hitbox.bottom))
		{
			Color* color = _render_target->GetPixel(x, y, _GD);
			if (color && color->A() > 0)
			{
				return true;
			}
		}
	}
	*/
	_render_target->Map(_context);
	float alpha = _render_target->GetPixel(pos.x, pos.y, _GD)->A();

	Color* col = _render_target->GetPixel(500, 700, _GD);
	if (alpha > 0.0F)
	{
		_render_target->Unmap(_context);
		return true;
	}
	_render_target->Unmap(_context);
	return false;
}