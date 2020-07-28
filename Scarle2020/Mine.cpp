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
	m_active = 2; //How long it takes to explode once triggered
	
	c_phys = new PhysicsComp(&m_pos, 0.3f, 0.2f, true);
	c_collider = new CollisionComp(16, 16);
}

Mine::Mine(const Mine& _orig) : Weapon(_orig)
{
	m_fake = _orig.m_fake;
	c_phys = new PhysicsComp(&m_pos, 0.3f, 0.2f, true);
	c_collider = new CollisionComp(16, 16);
}

void Mine::Tick(GameData* _GD)
{
	if (m_triggered)
	{
		m_time += _GD->m_dt;
		
		if (m_time > m_active)
		{
			if (!m_fake)
			{
				m_explode.pos = m_pos;
				m_explode.explode = true;
			}
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

}

bool Mine::Spawn(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD)
{
	return false;
}