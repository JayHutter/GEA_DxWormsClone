#include "pch.h"
#include "Projectile.h"
#include "GameData.h"

Projectile::Projectile(string _sprite, Vector2 _pos, float _width, float _height, bool _bounce, bool _gravity, float _damage, Vector2 _knockback,  float _explosion_rad, float _time, Worm* _owner, ID3D11Device* _DD) : ImageGO2D(_sprite, _DD)
{
	m_pos = _pos;
	m_active = _time;

	c_collider = new CollisionComp(_width, _height, _pos);
	c_phys = new PhysicsComp(&m_pos, 0.2f, _bounce, _gravity);

	m_damage = _damage;
	m_knockback = _knockback;

	m_explode.scale = _explosion_rad;
	m_explode.damage = _damage;
	m_explode.knockback = _knockback.x;

	m_owner = _owner;
}

void Projectile::Tick(GameData* _GD)
{
	m_time += _GD->m_dt;

	if (m_time > m_active)
	{
		//m_delete = true;
		m_explode.explode = m_explode.scale > 0;
	}
}

void Projectile::OnCollisionEnter(GameData* _GD, GameObject2D* _other)
{
	_other->AddHealth(-m_damage);

	if (m_explode.scale > 0)
	{
		m_explode.explode = true;
		m_explode.pos = m_pos;
	}
	else if (_other->GetPhysComp())
	{
		_other->GetPhysComp()->AddForce(m_knockback);
	}

	m_delete = true;
	
}

void Projectile::OnCollision(GameData* _GD, GameObject2D* _other)
{

}

void Projectile::OnCollisionExit(GameData* _GD, GameObject2D* _other)
{

}