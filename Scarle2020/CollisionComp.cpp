#include "pch.h"
#include "CollisionComp.h"

CollisionComp::CollisionComp(float _width, float _height, Vector2 _pos)
{
	SetHitbox(_width, _height, _pos);
}

CollisionComp::CollisionComp(float _width, float _height)
{
	SetHitbox(_width, _height);
}

void CollisionComp::SetHitbox(float _width, float _height, Vector2 _pos)
{
	hitbox.top = _pos.y -(_height / 2);
	hitbox.bottom = _pos.y + _height / 2;
	hitbox.left = _pos.x -(_width / 2);
	hitbox.right = _pos.x + _width / 2;
}

void CollisionComp::SetHitbox(float _width, float _height)
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
	auto alpha = _render_target->GetPixel(_pixel.x, (-_pixel.y) + collision_offset, _GD)->A();
	
	if (alpha > 0.0f)
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

//Test if side has collided with pixel
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

/*
Count the amount of pixels collided with on each side of the hitbox
{ TOP, BOTTOM, LEFT, RIGHT }
*/
std::array<int, 4> CollisionComp::TerrainCollsionV(RenderTarget* _render_target, ID3D11DeviceContext* _context, GameData* _GD, Vector2 _pos)
{
	//Amount pixels collided with on each side
	std::array<int, 4> pixels = { 0,0,0,0 };
	
	//Flip y coord as y=0 is bottom instead of top
	_pos.y *= -1;

	//for (int x = _pos.x + hitbox.left; x++; x < )
	for (int x = hitbox.left; x < hitbox.right; x++)
	{
		auto alpha = _render_target->GetPixel(x, (-hitbox.bottom) + collision_offset, _GD)->A();
		if (alpha > 0.0f)
		{
			pixels[0] ++;
		}
	
		alpha = _render_target->GetPixel(x, (-hitbox.top) + collision_offset, _GD)->A();
		if (alpha > 0.0f)
		{
			pixels[1] ++;
		}
	}

	for (int y = hitbox.bottom; y > hitbox.top; y--)
	{
		auto alpha = _render_target->GetPixel(hitbox.left, (-y) + collision_offset, _GD)->A();

		if (alpha > 0.0f)
		{
			pixels[2] ++;
		}
	
		alpha = _render_target->GetPixel(hitbox.right, (-y) + collision_offset, _GD)->A();
		if (alpha > 0.0f)
		{
			pixels[3] ++;
		}
	}

	return pixels;	
}

//Calculate the collision normal from the collision data
Vector2 CollisionComp::CalculateNormal(std::array<int, 4> _collisions)
{
	Vector2 normal = Vector2::Zero;

	float half_height = (hitbox.bottom - hitbox.top) / 2;
	float half_width = (hitbox.right - hitbox.left) / 2;

	if (_collisions[3] > _collisions[2])
	{
		normal.x = _collisions[3] * -1;
	}
	else if (_collisions[2] > _collisions[3])
	{
		normal.x = _collisions[2];
	}

	if (_collisions[1] > _collisions[0])
	{
		normal.y = _collisions[1];
	}
	else if (_collisions[0] > _collisions[1])
	{
		normal.y = _collisions[0] * -1;
	}

	//if the object is stuck in the terrain, shoot them upwards
	if (_collisions[0] > half_width && _collisions[1] > half_width &&
		_collisions[2] > half_height && _collisions[3] > half_height)
	{
		normal.x = 0;
		normal.y = -2 * (hitbox.bottom - hitbox.top);
	}
	//Prevent worms from climbing walls
	else if (_collisions[3] >= half_height)
	{
		normal.x = _collisions[3] * -5;
	}
	else if (_collisions[2] >= half_height)
	{
		normal.x = _collisions[2] * 5;
	}

	return normal;
}

void CollisionComp::UpdateHitbox(Vector2 _pos)
{
	float half_height = (hitbox.bottom - hitbox.top) / 2;
	float half_width = (hitbox.right - hitbox.left) / 2;

	hitbox.top = _pos.y - half_height;
	hitbox.bottom = _pos.y + half_height;

	hitbox.left = _pos.x - half_width;
	hitbox.right = _pos.x + half_width;
}

bool CollisionComp::Collided(RECT _other)
{
	return (hitbox.left < _other.right&& hitbox.right > _other.left &&
		hitbox.top < _other.bottom&& hitbox.bottom > _other.top);
}

RECT CollisionComp::Hitbox()
{
	return hitbox;
}

//Calculate normal if collided with anohter object
Vector2 CollisionComp::CalculateNormal(RECT _other)
{	
	Vector2 other_pos = Vector2(_other.left + ((_other.right - _other.left) / 2),
		_other.top + ((_other.bottom - _other.top) / 2));
	
	Vector2 pos = Vector2(hitbox.left + ((hitbox.right - hitbox.left) / 2),
		hitbox.top + ((hitbox.bottom - hitbox.top) / 2));

	Vector2 normal = pos - other_pos;	
	if (normal == Vector2::Zero)
	{
		return Vector2(0, -1);
	}

	return normal;
}

float CollisionComp::HitboxWidth()
{
	return hitbox.right - hitbox.left;
}

float CollisionComp::HitboxHeight()
{
	return hitbox.bottom - hitbox.top;
}