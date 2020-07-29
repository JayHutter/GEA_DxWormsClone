#include "pch.h"
#include "Healthbar.h"

Healthbar::Healthbar(int _maximum, Color _colour, Vector2 _pos, ID3D11Device* _GD) : ImageGO2D("Healthbar", _GD)
{
	m_region = _pos;
	m_max = _maximum;
	m_colour = _colour;
	m_origin = Vector2(0, 0);

	m_meter = new ImageGO2D("Health", _GD);
	m_meter->SetColour(_colour);
	m_meter->SetOrigin(Vector2(0, 0));
	
	m_text = new TextGO2D("0");
	m_text->SetScale(0.5f);
	m_text->SetColour(Color(Colors::Black));
}

void Healthbar::Draw(DrawData2D* _DD)
{
	UpdatePositions();	
	m_meter->Draw(_DD);

	ImageGO2D::Draw(_DD);
	m_text->Draw(_DD);
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
	m_text->SetText(std::to_string(m_health));
}

void Healthbar::UpdatePositions()
{
	m_pos = m_region + Vector2(0, 34 * (m_position - 1));
	m_meter->SetPos(m_pos + Vector2(37, 4));
	m_text->SetPos(m_pos + Vector2(5, 0));

	m_meter->SetScale(Vector2(m_percent, 1));
}