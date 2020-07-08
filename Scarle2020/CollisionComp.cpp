#include "CollisionComp.h"


void CollisionComp::AddHurtbox(Vector2 _pos_a, Vector2 _pos_b)
{
	hurt_boxes.push_back(CollisionBox{ _pos_a, _pos_b });
}


void CollisionComp::AddHitBox(Vector2 _pos_a, Vector2 _pos_b, float _dmg, Vector2 _kb)
{
	//hit_boxes.push_back(HitBox{ _pos_a, _pos_b, _dmg, _kb });
}

