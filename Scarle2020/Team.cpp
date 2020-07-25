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
	m_weapons[m_selection]->SetPos(m_worms[m_current]->GetPos());
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

void Team::UseWeapon(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD)
{
	if (m_available[m_selection] > 0 || m_available[m_selection] == -1)
	{
		auto weapon = m_weapons[m_selection]->Clone(_DD);
		weapon->Use(_GD, m_worms[m_current]);
		m_available[m_selection] -= (m_available[m_selection] != -1);
		_objects.push_back(weapon);

		//[m_selection]->Use(_GD, m_worms[m_current]);
	}
}

void Team::SelectWeapon(GameData* _GD, std::vector<GameObject2D*>& _objects)
{
	//m_selected_weapon = m_weapons[m_selection];
}

void Team::AimWeapon(GameData* _GD)
{
	m_weapons[m_selection]->Aim(_GD);
}

void Team::RenderHUD(DrawData2D* _DD)
{
	for (int i = 0; i < m_worms.size(); i++)
	{
		m_worms[i]->DrawHealth(_DD);
	}

	m_weapons[m_selection]->Draw(_DD);
}

void Team::DeleteWorm(Worm* _worm)
{
	auto end = m_worms.end();
	auto result = std::remove(m_worms.begin(), end, _worm);
	m_worms.erase(result, end);
}