#include "pch.h"
#include "Button.h"
#include "GameData.h"

Button::Button(Command _command, Vector2 _pos, Vector2 _scale, string _text, ID3D11Device* _GD) : ImageGO2D("Button", _GD)
{
	m_origin = Vector2(0, 0);
	m_com = _command;
	m_scale = _scale;

	m_pos = _pos;
	m_scale = _scale;
	
	m_hitbox.left = _pos.x;
	m_hitbox.right = _pos.x + (128 * m_scale.x);
	m_hitbox.top = _pos.y;
	m_hitbox.bottom = _pos.y + (128 * m_scale.y);

	m_disp = new TextGO2D("");
	m_disp->SetText(_text);

	Vector2 pos = m_pos;
	pos.x += (64 * m_scale.x) - (32 * _text.length());

	m_disp->SetPos(pos);// +Vector2(5, 0));
	m_disp->SetScale(2);
}

Command Button::Clicked(GameData* _GD)
{
	auto mouse = _GD->m_MS;

	if (mouse.x > m_hitbox.left && mouse.x < m_hitbox.right &&
		mouse.y > m_hitbox.top && mouse.y < m_hitbox.bottom)
	{
		ChangeColour(true);
		if (mouse.leftButton && m_time > m_delay && !m_locked)
		{
			m_time = 0;
			return m_com;
		}
		m_time += _GD->m_dt;
	}
	else
	{
		ChangeColour(false);
	}

	return Command::NONE;
}

void Button::Draw(DrawData2D* _DD)
{
	ImageGO2D::Draw(_DD);
	
	if (m_disp)
	{
		m_disp->Draw(_DD);
	}
}

void Button::SetState(bool _locked)
{
	m_locked = _locked;
}

void Button::ChangeColour(bool _lit)
{
	if (_lit)
	{
		if (m_locked)
		{
			m_colour = Colors::Red;
		}
		else
		{
			m_colour = Colors::Purple;
		}
	}
	else
	{
		if (m_locked)
		{
			m_colour = Colors::DarkRed;
		}
		else
		{
			m_colour = Colors::White;
		}
	}
}

void Button::SetText(string _text)
{
	m_disp->SetText(_text);
}