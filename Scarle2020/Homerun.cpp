#include "pch.h"
#include "Homerun.h"

Homerun::Homerun(ID3D11Device* _GD) : Weapon("bat", _GD)
{
	m_damage = 25;
	m_knockback = Vector2(1000, 0);
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
	if (_other == m_owner || m_end == true)
	{
		return;
	}

	if (dynamic_cast<Worm*>(_other))
	{
		m_end = true;
		m_knockback.x *= m_owner->Direction();

		_other->GetPhysComp()->AddForce(m_knockback);
		dynamic_cast<Worm*>(_other)->AddHealth(-m_damage);	
	}
}


void Homerun::Use(GameData* _GD, Worm* _owner)
{
	m_end = false;
	m_time = 0;
	m_knockback = Vector2(1000, 0);

	if (!c_collider)
	{
		c_collider = new CollisionComp(40, 30);
	}

	m_owner = _owner;
	m_pos = _owner->GetPos();
}