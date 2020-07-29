#include "pch.h"
#include "Mine.h"
#include "GameData.h"

Mine::Mine(int _fake_percent, ID3D11Device* _GD) : Weapon("Mine", _GD)
{
	int fake = std::rand() % 100;

	if (fake < _fake_percent)
	{
		m_fake = true;
	}

	m_explode.damage = 25;
	m_explode.knockback = 200;
	m_explode.scale = 0.6f;
	m_active = 3; //How long it takes to explode once triggered
	m_chargeable = true;
	
	c_phys = new PhysicsComp(&m_pos, 0.3f, 0.2f, true);
	c_collider = new CollisionComp(16, 16);
	m_aim_sprite = "WormMine";

	m_invincible = false;
	m_health = 20;
}

Mine::Mine(const Mine& _orig) : Weapon(_orig)
{
	m_fake = _orig.m_fake;
	c_phys = new PhysicsComp(&m_pos, 0.3f, 0.2f, true);
	c_collider = new CollisionComp(16, 16);
}

void Mine::Tick(GameData* _GD)
{
	//Dont let it explode if its moving
	if (c_phys->GetVel().x > 10 || c_phys->GetVel().y > 10)
	{
		m_triggered = false;
	}

	if (m_triggered)
	{
		m_colour = Colors::Red;
		m_time += _GD->m_dt;
		
		if (m_time > m_active)
		{
			if (!m_fake)
			{
				m_explode.pos = m_pos;
				m_explode.explode = true;
			}
			else
			{
				m_colour = Colors::Gray;
			}
		}

		if (m_owner != nullptr)
		{
			m_end = true;
		}
	}
}

void Mine::OnCollisionEnter(GameData* _GD, GameObject2D* _other)
{
	if (dynamic_cast<Worm*>(_other))
	{
		m_triggered = true;
	}
}

void Mine::OnCollision(GameData* _GD, GameObject2D* _other)
{

}

void Mine::OnCollisionExit(GameData* _GD, GameObject2D* _other)
{

}

Mine* Mine::Clone(ID3D11Device* _GD)
{
	Mine* clone = new Mine(*this);
	clone->SetSprite(m_filename, _GD);
	return clone;
}

void Mine::Aim(GameData* _GD)
{

}

void Mine::Use(GameData* _GD, Worm* _owner, float _charge)
{
	Vector2 force = Vector2(_GD->m_MS.x, _GD->m_MS.y);
	force -= _owner->GetPos();
	force.Normalize();

	m_pos = _owner->GetPos();
	m_pos.x += (force.x * 15);

	force.x *= _charge * 500;
	force.y *= _charge * 700;

	c_phys->AddForce(force);

	m_owner = _owner;
}

bool Mine::Spawn(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD)
{
	return false;
}