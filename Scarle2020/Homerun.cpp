#include "pch.h"
#include "Homerun.h"

Homerun::Homerun(ID3D11Device* _GD) : Weapon("bat", _GD)
{
}

void Homerun::Tick(GameData* _GD)
{
	m_time += _GD->m_dt;
	if (m_time > m_active)
	{
		m_end = true;
	}
}

void Homerun::OnCollision(GameData* _GD, GameObject2D* _other)
{
	if (_other == m_owner)
	{
		return;
	}

	if (dynamic_cast<Worm*>(_other))
	{
		_other->GetPhysComp()->AddForce(m_knockback);
		dynamic_cast<Worm*>(_other)->AddHealth(-m_damage);
	}
}


void Homerun::Use(GameData* _GD, Worm* _owner)
{
	m_owner = _owner;
	c_collider = new CollisionComp(40, 30);
	m_pos = _owner->GetPos();
}