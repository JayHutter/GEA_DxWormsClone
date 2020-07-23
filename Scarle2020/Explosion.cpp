#include "pch.h"
#include "Explosion.h"
#include "GameData.h"

Explosion::Explosion(Worm* _owner, ID3D11Device* _GD) : ImageGO2D("explosion", _GD)
{
	m_owner = _owner;
	c_collider = new CollisionComp(128, 128);
}


void Explosion::SetData(float _size, Vector2 _pos, float _damage, float _knockback)
{
	m_scale = Vector2(_size, _size);
	m_pos = _pos;
	m_damage = _damage;
	m_knockback = _knockback;
	c_collider->SetHitbox(128 * _size, 128 * _size);
}

void Explosion::SetData(ExplosionData _data)
{
	m_scale = Vector2(_data.scale, _data.scale);
	m_pos = _data.pos;
	m_damage = _data.damage;
	m_knockback = _data.knockback;
	c_collider->SetHitbox(128 * _data.scale, 128 * _data.scale);
}
	

void Explosion::OnCollisionEnter(GameData* _GD, GameObject2D* _other)
{
	_other->AddHealth(-m_damage);
	auto phys = _other->GetPhysComp();
	if (phys)
	{
		Vector2 force = _other->GetPos() - m_pos;
		force.Normalize();
		force *= m_knockback;
		phys->AddForce(force);
	}

	m_collided.push_back(_other);
}

void Explosion::OnCollision(GameData* _GD, GameObject2D* _other)
{
}

void Explosion::OnCollisionExit(GameData* _GD, GameObject2D* _other)
{
	RemoveFromCollided(_other);
}

void Explosion::Tick(GameData* _GD)
{
	m_time += _GD->m_dt;
	if (m_time > m_exist)
	{
		m_delete = true;
	}
}