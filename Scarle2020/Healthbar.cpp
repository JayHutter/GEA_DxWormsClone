#include "pch.h"
#include "Healthbar.h"
#include "Worm.h"

Healthbar::Healthbar(int _maximum, Color _colour, Vector2 _pos, ID3D11Device* _GD) : ImageGO2D("Healthbar", _GD)
{
	m_max = _maximum;
	m_colour = _colour;
	m_pos = _pos;
	m_origin = Vector2(0, 0);

	m_meter = new ImageGO2D("Health", _GD);
	m_meter->SetColour(_colour);
	m_meter->SetPos(_pos + Vector2(37, 4));
	m_meter->SetOrigin(Vector2(0, 0));
	
	m_text = new TextGO2D("0");
	m_text->SetScale(0.5f);
	m_text->SetPos(_pos + Vector2(5, 0));
	m_text->SetColour(Color(Colors::Black));

	m_occlusion.left = m_pos.x - 10;
	m_occlusion.left = m_pos.x + 266;
	m_occlusion.bottom = m_pos.y + 42;
	m_occlusion.top = m_pos.y -10;
}

void Healthbar::Draw(DrawData2D* _DD)
{
	m_meter->SetScale(Vector2(m_percent, 1));
	m_meter->Draw(_DD);

	ImageGO2D::Draw(_DD);
	//m_text->Draw(_DD);
}

void Healthbar::SetHealth(int _health)
{
	m_health = _health; 
	m_percent = (m_health / m_max);
}

void Healthbar::SetPercent(float _percent)
{
	m_percent = _percent;

	m_health = m_max * m_percent;
}

void Healthbar::SetPosition(int _position)
{
	m_position = _position;
	m_text->SetText(std::to_string(m_position));
}

void Healthbar::Occlusion(CollisionComp* _collider, ID3D11Device* _GD)
{
	if (!_collider)
	{
		return;
	}

	if (_collider->Collided(m_occlusion))
	{
		SetSprite("HealthbarTrans", _GD);
		m_meter->SetSprite("HealthTrans", _GD);
	}
	else
	{
		SetSprite("Healthbar", _GD);
		m_meter->SetSprite("Health", _GD);
	}

	m_origin = Vector2(0, 0);
	m_meter->SetOrigin(Vector2(0, 0));
}
