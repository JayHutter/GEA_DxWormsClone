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
	//m_weapons[m_selection]->SetPos(m_worms[m_current]->GetPos());
	for (auto weapon : m_weapons)
	{
		weapon->SetPos(m_worms[m_current]->GetPos());
	}
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
	m_weapons.push_back(new MissileLauncher(_GD));
	m_weapons.push_back(new Homerun(_GD));

	m_weapons.push_back(new Grenade(1, 0.5f, 5, 25, 200, 0, "Grenade", _GD));
	m_weapons.push_back(new Grenade(4, 0.25f, 3, 50, 700, 0, "Holy", _GD));
	m_weapons.push_back(new Grenade(1, 0.5f, 5, 20, 200, 10, "Frag", _GD));
	m_weapons.push_back(new Grenade(1, 1, 8, 30, 300, 4, "Banana", _GD));

	m_weapons.push_back(new Dropper(3, 0, false, 2, 40, 350, 7, "TNT", _GD));
	m_weapons.push_back(new Dropper(5, 0, true, 0, 0, 0, 4, "Bowling", _GD));
	m_weapons.push_back(new Mine(0, _GD));

	m_weapons.push_back(new Teleport(_GD));

	m_available.push_back(-1);
	m_available.push_back(5);

	m_available.push_back(10);
	m_available.push_back(2);
	m_available.push_back(5);
	m_available.push_back(3);

	m_available.push_back(3);
	m_available.push_back(1);
	m_available.push_back(5);

	m_available.push_back(5);

	weapon_count = m_weapons.size();
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

	if ((m_available[m_selection] > 0 || m_available[m_selection] == -1) && m_can_attack)
	{
		if (m_weapons[m_selection]->Chargeable() && _GD->m_MS.leftButton)
		{
			m_charging = true;
			m_charge += _GD->m_dt;
		}
		else if (_GD->m_MS.leftButton || m_charging)
		{
			auto weapon = m_weapons[m_selection]->Clone(_DD);
			m_available[m_selection] -= (m_available[m_selection] != -1);
			_objects.push_back(weapon);
			weapon->Use(_GD, m_worms[m_current], m_charge);
			m_charging = false;
			m_can_attack = false;
			m_charge = false;
		}
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

	//m_weapons[m_selection]->Draw(_DD);
}

void Team::DeleteWorm(Worm* _worm)
{
	auto end = m_worms.end();
	auto result = std::remove(m_worms.begin(), end, _worm);
	m_worms.erase(result, end);
}

void Team::ChangeWormSprite(GameData* _GD, ID3D11Device* _DD)
{
	for (auto worm : m_worms)
	{
		if (worm != m_worms[m_current] || !m_can_attack)
		{
			worm->SetSprite("Worm", _DD);
		}
		else
		{
			worm->SetSprite(m_weapons[m_selection]->AimSprite(), _DD);
		}
	}
}

//For changing value at the end of a turn
void Team::EndTurn(GameData* _GD, ID3D11Device* _DD)
{
	//Reset all sprites
	for (auto worm : m_worms)
	{
		worm->SetSprite("Worm", _DD);
	}

	m_can_attack = true;
}