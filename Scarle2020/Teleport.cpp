#include "pch.h"
#include "Teleport.h"
#include "GameData.h"

Teleport::Teleport(ID3D11Device* _GD) : Weapon("Teleport", _GD)
{
	m_active = 0.3;
}

Teleport::Teleport(const Teleport& _orig) : Weapon(_orig)
{
	m_target = _orig.m_target;
	m_blocked = _orig.m_blocked;
}

Teleport* Teleport::Clone(ID3D11Device* _GD)
{
	Teleport* clone = new Teleport(*this);
	clone->SetSprite(m_filename, _GD);
	return clone;
}

void Teleport::Tick(GameData* _GD)
{
	if (m_collided.size() > 0)
	{
		m_blocked = true;
	}

	m_time += _GD->m_dt;
	if (m_time > m_active)
	{
		if (!m_blocked)
		{
			m_owner->SetPos(m_pos);
		}
		m_delete = true;
	}
}

void Teleport::Use(GameData* _GD, Worm* _owner, float _charge)
{
	if (!c_collider)
	{
		c_collider = new CollisionComp(32, 32);
	}

	m_owner = _owner;
	m_pos = Vector2(_GD->m_MS.x, _GD->m_MS.y);
}

void Teleport::OnCollisionEnter(GameData* _GD, GameObject2D* _other)
{
	m_collided.push_back(_other);
}

void Teleport::OnCollisionExit(GameData* _GD, GameObject2D* _other)
{
	RemoveFromCollided(_other);
}