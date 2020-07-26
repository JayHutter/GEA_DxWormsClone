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
