#include "pch.h"
#include "Team.h"

Team::Team(ID3D11Device* _GD, int _worms, Color _colour, std::vector<GameObject2D*> &_objects)
{
	for (int i = 0; i < _worms; i++)
	{
		Worm* worm = new Worm(_GD, _colour, "Worm " + std::to_string(i));
		m_worms.push_back(worm);
		_objects.push_back(worm);

		//TEMP POS SETTING
		if (_colour == Color(Colors::Red))
		{
			worm->SetPos(Vector2(50 + (i * 100), 100));
		}
		else if (_colour == Color(Colors::Blue))
		{
			worm->SetPos(Vector2(100 + (i * 100), 500));
		}
		else if (_colour == Color(Colors::Green))
		{
			worm->SetPos(Vector2(900 + (i * 100), 150));
		}
		else if (_colour == Color(Colors::Orange))
		{
			worm->SetPos(Vector2(700 + (i * 100), 500));
		}
	}

	SetupWeapons(_GD);

	m_colour = _colour;
}

Team::~Team()
{
	for (auto w : m_worms)
	{
		w = nullptr; // Worm also in object stack and is deleted from there
	}

	m_worms.clear();
}

void Team::Tick(GameData* _GD)
{
}

Worm* Team::GetWorm()
{
	return m_worms[m_current];
}

void Team::CycleWorm()
{
	m_current++;
	m_current %= m_worms.size();
}

void Team::SetupWeapons(ID3D11Device* _GD)
{
	m_weapons[0] = new Homerun(_GD);
	m_available[0] = -1;
}

void Team::CycleWeapon(int _dir)
{
	m_selection += _dir;
	if (m_selection < 0)
	{
		m_selection += weapon_count;
	}
	m_selection %= weapon_count;
}

void Team::UseWeapon(GameData* _GD, std::vector<GameObject2D*>& _objects)
{
	if (m_available[m_selection] > 0 || m_available[m_selection] == -1)
	{
		_objects.push_back(m_weapons[m_selection]);
		m_weapons[m_selection]->Use(_GD, m_worms[m_current]);
	}
}

void Team::RenderHUD(DrawData2D* _DD)
{
	for (int i = 0; i < m_worms.size(); i++)
	{
		if (i == m_current)
		{
			m_worms[m_current]->DrawName(_DD);
		}
		m_worms[i]->DrawHealth(_DD);
	}
}