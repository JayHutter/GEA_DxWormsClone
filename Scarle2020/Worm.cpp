#include "pch.h"
#include "Worm.h"
#include "DrawData2D.h"

Worm::Worm(ID3D11Device* _GD, Color _colour, string _name) : ImageGO2D("worm", _GD)
{
	c_phys = new PhysicsComp(&m_pos, 0.5f, 0, true);
	c_collider = new CollisionComp(20, 24);

	m_team_colour = _colour;
	m_name = _name;

	//m_hud = new TextGO2D("HUD");

	m_health_display = new TextGO2D("HEALTH");
	m_health_display->SetScale(0.25f);
	m_health_display->SetColour(m_team_colour);

	m_name_display = new TextGO2D("NAME");
	m_name_display->SetScale(0.25f);
	m_name_display->SetColour(m_team_colour);

	m_invincible = false;

	m_explode.scale = 0.5f;
	m_explode.damage = 15;
	m_explode.knockback = 50;
}

void Worm::Tick(GameData* _GD)
{
	UpdateHUD();
}

void Worm::Move(float _dir)
{
	if (m_move)
	{
		c_phys->SetVelocityX(50 * _dir);

		m_dir = 1;
		if (_dir < 0)
		{
			m_dir = -1;
		}
	}
}


void Worm::DrawHUD(DrawData2D* _DD)
{
	//REMOVE
}

//Private
void Worm::UpdateHUD()
{
	if (m_health_display)
	{
		m_health_display->SetText(std::to_string(m_health));
		Vector2 pos = m_pos;
		pos.y -= 40;
		m_health_display->SetPos(pos);
	}
	if (m_name_display)
	{
		m_name_display->SetText(m_name);
		Vector2 pos = m_pos;
		pos.y -= 50;
		m_name_display->SetPos(pos);
	}
}

void Worm::DrawHealth(DrawData2D* _DD)
{
	m_health_display->Draw(_DD);
}

void Worm::DrawName(DrawData2D* _DD)
{
	m_name_display->Draw(_DD);
}

void Worm::Kill(ID3D11Device* _GD)
{
	SetSprite("Grave", _GD);
	m_colour = Colors::White + m_team_colour;
	m_dead = true;
	m_health = 0;
	c_phys->SetFrictionVal(1);
}
bool Worm::IsDead()
{
	return m_dead;
}
