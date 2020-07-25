#include "pch.h"
#include "Weapon.h"

Weapon::Weapon(const Weapon& orig) : ImageGO2D(orig)
{
	m_damage = orig.m_damage;
	m_knockback = orig.m_knockback;
}

void Weapon::Tick(GameData* _GD)
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

void Weapon::SetOwner(Worm* _owner)
{
	m_owner = _owner;
}