#include "pch.h"
#include "InputBox.h"
#include "GameData.h"

InputBox::InputBox(Command _command, Vector2 _pos, Vector2 _scale, int _default, int _min, int _max, ID3D11Device* _GD) : Button(_command, _pos, _scale, std::to_string(_default), _GD)
{
	m_value = _default;

	m_min = _min;
	m_max = _max;

	if (_min <= 0)
	{
		m_min = 1;
	}
	if (_max > 10)
	{
		m_max = 10;
	}

	if (m_max < m_min)
	{
		m_max = m_min + 1;
	}
}

Command InputBox::Clicked(GameData* _GD)
{
	auto mouse = _GD->m_MS;

	if (mouse.x > m_hitbox.left && mouse.x < m_hitbox.right &&
		mouse.y > m_hitbox.top && mouse.y < m_hitbox.bottom)
	{
		GetInputValue(_GD);
	}

	return Button::Clicked(_GD);
}

void InputBox::GetInputValue(GameData* _GD)
{
	auto key = _GD->m_KBS;

	if (key.D0 || key.NumPad0)
	{
		m_value = 10;
	}
	if (key.D1 || key.NumPad1)
	{
		m_value = 1;
	}
	if (key.D2 || key.NumPad2)
	{
		m_value = 2;
	}
	if (key.D3 || key.NumPad3)
	{
		m_value = 3;
	}
	if (key.D4 || key.NumPad4)
	{
		m_value = 4;
	}
	if (key.D5 || key.NumPad5)
	{
		m_value = 5;
	}
	if (key.D6 || key.NumPad6)
	{
		m_value = 6;
	}
	if (key.D7 || key.NumPad7)
	{
		m_value = 7;
	}
	if (key.D8 || key.NumPad8)
	{
		m_value = 8;
	}
	if (key.D9 || key.NumPad9)
	{
		m_value = 9;
	}

	NormaliseValue();

	m_disp->SetText(std::to_string(m_value));
}

void InputBox::SetValue(int _val)
{
	m_value = _val - 1;

	if (m_value < 0)
	{
		m_value += 10;
	}

	m_value = (m_value % 10) + 1;

	NormaliseValue();
	m_disp->SetText(std::to_string(m_value));
}

void InputBox::NormaliseValue()
{
	if (m_value < m_min)
	{
		m_value = m_max;
	}
	
	if (m_value > m_max)
	{
		m_value = m_min;
	}
}