#include "pch.h"
#include "Weapon.h"


void Weapon::Tick(GameData* _GD)
{
}

void Weapon::OnCollision(GameData* _GD, GameObject2D* _other)
{
}

float Weapon::Damage()
{
	return m_damage;
}

Vector2 Weapon::Knockback()
{
	return m_knockback;
}

bool Weapon::EndTurn()
{
	return m_end;
}

DestructionMask* Weapon::Destruction()
{
	return m_destruction;
}