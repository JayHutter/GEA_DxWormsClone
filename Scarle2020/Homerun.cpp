#include "pch.h"
#include "Homerun.h"

Homerun::Homerun(ID3D11Device* _GD) : Weapon("Bat", _GD)
{
	m_damage = 25;
	m_knockback = Vector2(500, 0);
	m_active = 0.05f;
	m_aim_sprite = "WormBat";
}

Homerun::Homerun(const Homerun& orig) : Weapon(orig)
{
}

Homerun* Homerun::Clone(ID3D11Device* _GD)
{
	Homerun* clone = new Homerun(*this);
	clone->SetSprite(m_filename, _GD);
	return clone;
}

void Homerun::Tick(GameData* _GD)
{
	m_time += _GD->m_dt;
	if (m_time > m_active)
	{
		m_end = true;
		m_delete = true;
	}
}

void Homerun::OnCollisionEnter(GameData* _GD, GameObject2D* _other)
{
	if (_other == m_owner || m_end == true || !m_owner)
	{
		return;
	}

	if (dynamic_cast<Worm*>(_other))
	{
		_other->GetPhysComp()->AddForce(m_knockback);
		_other->AddHealth(-m_damage);	
	}

	m_collided.push_back(_other);
}

void Homerun::OnCollision(GameData* _GD, GameObject2D* _other)
{
}

void Homerun::OnCollisionExit(GameData* _GD, GameObject2D* _other)
{
	RemoveFromCollided(_other);
}


void Homerun::Use(GameData* _GD, Worm* _owner, float _charge)
{
	//Vector2 dir = Vector2(_GD->m_MS.x, _GD->m_MS.y) - _owner->GetPos();
	//dir.Normalize();
	//m_knockback = Vector2(1000, 0);
	//m_knockback = dir * 1000;

	m_owner = _owner;
	m_pos = _owner->GetPos();

	if (!c_collider)
	{
		c_collider = new CollisionComp(40, 30, m_pos);
	}
}

void Homerun::Aim(GameData* _GD)
{
	Vector2 direction = Vector2(_GD->m_MS.x, _GD->m_MS.y);
	direction -= m_pos;
	direction.Normalize();

	direction *= 500;
	m_knockback = direction;
}