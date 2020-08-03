#include "pch.h"
#include "Team.h"

Team::Team(ID3D11Device* _GD, int _worms, Color _colour, int _port, std::vector<GameObject2D*> &_objects)
{
	for (int i = 0; i < _worms; i++)
	{
		Worm* worm = new Worm(_GD, _colour, "Worm " + std::to_string(i));
		m_worms.push_back(worm);
		_objects.push_back(worm);
	}

	SetupWeapons(_GD);

	m_colour = _colour;

	UpdateHealth();

	Vector2 hud_pos = Vector2(20, 650 - (34 * _port));
	m_hud = new Healthbar(m_total_health, m_colour, hud_pos, _GD);
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

	CheckDamage();
}

void Team::Update(GameData* _GD)
{
	UpdateHealth();
	m_hud->SetHealth(m_total_health);
}

void Team::CycleWorm()
{
	if (m_total_health <= 0)
	{
		return;
	}

	for (int i = m_current +1; i < m_current + m_worms.size(); i++)
	{
		i %= m_worms.size();
		if (!m_worms[i]->IsDead())
		{
			m_current = i;
			return;
		}
	}
}

void Team::SetupWeapons(ID3D11Device* _GD)
{
	m_weapons.push_back(new MissileLauncher(_GD));
	m_weapons.push_back(new Homerun(_GD));

	m_weapons.push_back(new Grenade(1, 0.5f, 5, 25, 200, 0, "Grenade", _GD));
	m_weapons.push_back(new Grenade(4, 0.25f, 3, 50, 700, 0, "Holy", _GD));
	m_weapons.push_back(new Grenade(1, 0.5f, 5, 20, 200, 6, "Frag", _GD));
	m_weapons.push_back(new Grenade(1, 1, 8, 30, 300, 4, "Banana", _GD));

	m_weapons.push_back(new Dropper(3, 0, false, 2, 40, 350, 7, "TNT", _GD));
	m_weapons.push_back(new Dropper(1, 0, true, 0, 0, 0, 4, "Bowling", _GD));
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

bool Team::UseWeapon(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD)
{

	if (m_worms[m_current]->GetPhysComp()->AirTime() >= 0.15f)
	{
		return false;
	}

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
			return true;
		}
	}

	return false;
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

	m_hud->Draw(_DD);
}

void Team::RenderWormHUD(DrawData2D* _DD)
{
	m_worms[m_current]->DrawName(_DD);
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
		if (worm->IsDead())
		{
			worm->SetSprite("Grave", _DD);
		}
		else if (worm != m_worms[m_current] || !m_can_attack)
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
void Team::OnEndTurn(ID3D11Device* _DD)
{
	//Reset all sprites
	for (auto worm : m_worms)
	{
		if (worm->IsDead())
		{
			worm->SetSprite("Grave", _DD);
			continue;
		}
		worm->SetSprite("Worm", _DD);
	}
}

void Team::UpdateHealth()
{
	float health = 0;
	for (auto worm : m_worms)
	{
		health += worm->Health();
	}

	m_total_health = health;
}

void Team::SetPlacing(int _placing)
{
	m_placing = _placing;
	m_hud->SetPosition(_placing);
}

void Team::TriggerEndTurn(bool _end)
{
	m_end = _end;
}

void Team::OnStartTrun()
{
	m_end = false;
	m_can_attack = true;

	CycleWorm();
	m_start_health = m_worms[m_current]->Health();
}

void Team::Control(GameData* _GD, ID3D11Device* _DD)
{
	auto worm = m_worms[m_current];

	if (worm->IsDead())
	{
		//CycleWorm();
		return;
	}

	if (m_charging)
	{
		return;
	}

	if (worm->GetPhysComp()->AirTime() < 0.15f)
	{
		worm->Move(_GD->m_KBS.D + (_GD->m_KBS.A * -1));

		if (_GD->m_KBS_tracker.IsKeyPressed((Keyboard::Space)))
		{
			Vector2 force = Vector2(0, -300);
			if (_GD->m_KBS.D)
			{
				force.x = 300;
			}
			else if (_GD->m_KBS.A)
			{
				force.x = -300;
			}

			worm->GetPhysComp()->AddForce(force);
		}
	}

	CycleWeapon(_GD->m_KBS_tracker.IsKeyPressed(Keyboard::E) + (-1 * _GD->m_KBS_tracker.IsKeyPressed(Keyboard::Q)));
}

bool Team::AllWormsDead()
{
	int dead = 0;
	for (auto worm : m_worms)
	{
		if (worm->IsDead())
		{
			dead++;
		}
	}

	if (dead == m_worms.size())
	{
		return true;
	}

	return false;
}

bool Team::SetScore(int _score)
{
	if (m_score == 1 && AllWormsDead())
	{
		m_score = _score;
		return true;
	}

	return false;
}

int Team::GetScore()
{
	return m_score;
}

void Team::CheckDamage()
{
	float health = m_worms[m_current]->Health();

	if (health < m_start_health)
	{
		m_end = true;
	}
}