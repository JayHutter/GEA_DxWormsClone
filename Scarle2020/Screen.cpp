#include "pch.h"
#include "Screen.h"

Screen::Screen(ID3D11Device* _GD)
{
	m_d3d11device = _GD;
}

void Screen::QuitGame(GameData* _GD)
{
	if (_GD->m_KBS_tracker.IsKeyPressed(Keyboard::Escape))
	{
		m_delete = true;
	}
}