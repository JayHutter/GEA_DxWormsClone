#include "pch.h"
#include "Worm.h"
#include "DrawData2D.h"

Worm::Worm(ID3D11Device* _GD, Color _colour, string _name) : ImageGO2D("worm", _GD)
{
	c_phys = new PhysicsComp(&m_pos, 0.5f, 0, true);
	c_collider = new CollisionComp(20, 24);

	m_colour = _colour;
	m_name = _name;

	m_hud = new TextGO2D("HUD");
	m_hud->SetScale(0.25f);
	m_hud->SetColour(m_colour);
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
	}
}

void Worm::DrawHUD(DrawData2D* _DD)
{
	if (m_hud)
	{
		m_hud->Draw(_DD);
	}
}


//Private

void Worm::UpdateHUD()
{
	if (m_hud)
	{
		string hud_text = m_name + "\n" + std::to_string(m_health);
		m_hud->SetText(hud_text);
		Vector2 pos = m_pos;
		pos.y -= 50;
		m_hud->SetPos(pos);
	}
}