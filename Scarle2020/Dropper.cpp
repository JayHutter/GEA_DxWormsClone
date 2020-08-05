#include "pch.h"
#include "Dropper.h"
#include "GameData.h"
#include "Stage.h"

Dropper::Dropper(float _damage, float _bounce, bool _scale_damage, float _explosion_rad, float _exp_damage, float _exp_knockback, float _time, string _sprite, ID3D11Device* _GD) : Weapon(_sprite, _GD)
{
	m_damage = _damage;
	m_base_damage = _damage;

	m_explode.scale = _explosion_rad;
	m_explode.damage = _exp_damage;
	m_explode.knockback = _exp_knockback;

	m_bounce = _bounce;
	m_active = _time;
	m_scale_damage = _scale_damage;
	m_aim_sprite = "Worm" + _sprite;

	m_prevent_swap = true;
}

Dropper::Dropper(const Dropper& _orig) : Weapon(_orig)
{
	m_bounce = _orig.m_bounce;
	m_scale_damage = _orig.m_scale_damage;
}

void Dropper::Tick(GameData* _GD)
{
	m_time += _GD->m_dt;
	if (m_time > m_active)
	{
		m_end = true;
		m_delete = true;
		if (m_explode.scale > 0)
		{
			m_explode.pos = m_pos;
			m_explode.explode = true;
		}
	}

	if (m_scale_damage)
	{
		m_damage = m_base_damage * (c_phys->GetVel().y / 20);
	}
}

void Dropper::OnCollisionEnter(GameData* _GD, GameObject2D* _other)
{
	if (m_bounce < 0.1 && dynamic_cast<Stage*>(_other) && c_phys->GetVel().y > 0)
	{
		c_phys->SetVelocutyY(0);
	}

	_other->AddHealth(-m_damage);
	m_collided.push_back(_other);
}
void Dropper::OnCollision(GameData* _GD, GameObject2D* _other)
{

}
void Dropper::OnCollisionExit(GameData* _GD, GameObject2D* _other)
{
	RemoveFromCollided(_other);
}

Dropper* Dropper::Clone(ID3D11Device* _GD)
{
	Dropper* clone = new Dropper(*this);
	clone->SetSprite(m_filename, _GD);
	return clone;
}

void Dropper::Aim(GameData* _GD)
{

}

void Dropper::Use(GameData* _GD, Worm* _owner, float _charge)
{
	if (!c_collider)
	{
		c_collider = new CollisionComp(16, 16);
	}
	if (!c_phys)
	{
		c_phys = new PhysicsComp(&m_pos, 1, m_bounce, true);
	}

	m_pos = _owner->GetPos();
	m_pos.x += 20 * _owner->Direction();
}

bool Dropper::Spawn(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD)
{
	return false;
}