#include "pch.h"
#include "GameObject2D.h"
#include "gamedata.h"
#include "DrawData2D.h"
#include "helper.h"

using namespace DirectX;

GameObject2D::GameObject2D()
{
	m_colour = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_pos = Vector2::Zero;
	m_rotation = 0.0f;
	m_scale = Vector2::One;
}

PhysicsComp* GameObject2D::GetPhysComp()
{
	return c_phys;
}

void GameObject2D::AddHealth(int _health)
{
	m_health += _health;
}

//If the passed object previously collide with this object
bool GameObject2D::IsCollided(GameObject2D* _obj)
{
	auto it = std::find(m_collided.begin(), m_collided.end(), _obj);
	if (it != m_collided.end())
	{
		return true;
	}
	return false;
}

void GameObject2D::RemoveFromCollided(GameObject2D* _obj)
{
	auto end = m_collided.end();
	auto result = std::remove(m_collided.begin(), end, _obj);
	m_collided.erase(result, end);
}

void GameObject2D::CheckHealth(float _gt)
{
	if (m_health < 0)
	{
		m_health = 0;
	}

	if (!m_invincible && m_health <= 0)
	{
		TriggerDeath(_gt);
	}
}

void GameObject2D::TriggerDeath(float _gt)
{
	m_death_timer += _gt;
	if (m_death_timer > 2)
	{
		m_explode.pos = m_pos;
		m_explode.explode = true;
		m_invincible = true;
	}
}