#include "pch.h"
#include "Weapon.h"

Weapon::Weapon(const Weapon& _orig) : ImageGO2D(_orig)
{
	m_damage = _orig.m_damage;
	m_knockback = _orig.m_knockback;
	m_active = _orig.m_active;
}

void Weapon::Tick(GameData* _GD)
{
}

bool Weapon::Chargeable()
{
	return m_chargeable;
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

string Weapon::AimSprite()
{
	return m_aim_sprite;
}
