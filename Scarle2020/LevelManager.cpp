#include "pch.h"
#include "LevelManager.h"
#include "DrawData2D.h"
#include "GameData.h"
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

LevelManager::LevelManager(ID3D11Device* _GD)
{
	m_objects.reserve(100);
	m_destruction.reserve(100);

	m_d3d11device = _GD;

	m_time_display = new TextGO2D("0");
	m_time_display->SetPos(Vector2(280, 600));
	m_game_timer = new TextGO2D("15:00");
	m_game_timer->SetPos(Vector2(275, 650));
	m_game_timer->SetScale(0.5f);
	m_sea = new ImageGO2D("Sea", _GD);
	m_sea->SetOrigin(Vector2(0, 0));
	m_sea->SetPos(Vector2(0, m_water_height));
}

LevelManager::~LevelManager()
{
	for (auto obj : m_objects)
	{
		if (obj)
		{
			delete obj;
			obj = nullptr;
		}
	}

	m_objects.clear();

	m_teams.clear();

	for (auto d : m_destruction)
	{
		if (d)
		{
			delete d;
			d = nullptr;
		}
		
	}

	m_destruction.clear();
	
	m_teams.clear();
}

void LevelManager::SetupLevel(string _name, int _teams, ID3D11Device* _GD)
{
	for (int i = 0; i < _teams; i++)
	{
		m_teams.push_back(Team(_GD, 4, default_colors[i], i, m_objects));
	}

	auto mine = new Mine(50, _GD);
	mine->SetPos(Vector2(600, 500));
	m_objects.push_back(mine);
	m_time_display = new TextGO2D("0");
	m_time_display->SetPos(Vector2(280, 600));
	m_game_timer = new TextGO2D("15:00");
	m_game_timer->SetPos(Vector2(275, 650));
	m_game_timer->SetScale(0.5f);
	m_stage = new Stage(_GD, _name);
	m_sea = new ImageGO2D("Sea", _GD);
	m_sea->SetOrigin(Vector2(0, 0));
	m_sea->SetPos(Vector2(0, m_water_height));
	m_score = m_teams.size();
	m_time_display->SetColour(m_teams[m_active].Colour());
}

//Load level from json file
void LevelManager::SetupLevel(string _name, int _teams, int _worms)
{
	std::ifstream file(_name + ".json");
	if (!file.is_open())
	{
		return;
	}
	nlohmann::json level;
	file >> level;
	
	m_game_time = level["time"].get<int>();
	m_water_height = level["water"].get<int>();

	m_stage = new Stage(m_d3d11device, _name);

	auto colours = level["colour"];
	for (int i = 0; i < _teams; i++)
	{
		auto c = colours[i];
		Color colour = { c["r"].get<float>(), c["g"].get<float>(), c["b"].get<float>() };
		m_teams.push_back(Team(m_d3d11device, _worms, colour, i, m_objects));
	}

	auto objects = level["objects"];
	for (auto& obj : objects)
	{
		StageObject* stg_obj = new StageObject(obj["sprite"].get<string>(), m_d3d11device);
		stg_obj->SetPos(Vector2(obj["xPos"].get<float>(), obj["yPos"].get<float>()));
		m_stage->AddSolid(stg_obj);
	}

	auto mines = level["mines"];
	for (auto& m : mines)
	{
		Mine* mine = new Mine(m["chance"].get<int>(), m_d3d11device);
		mine->SetPos(Vector2(m["xPos"].get<float>(), m["yPos"].get<float>()));
		m_objects.push_back(mine);
	}
}

void LevelManager::Tick(GameData* _GD)
{
	for (auto &team : m_teams)
	{
		team.Update(_GD);
	}
	
	//if (m_teams[m_active].EndTurn() || m_timer <= 0)
	//{
	//	CycleTeam();
	//}

	//HudOcclusion();
	//WinCondition();
}

void LevelManager::Update(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	switch (m_state)
	{
	case GameState::PLAYING:
		Play(_GD, _terrain, _context);
		break;
	case GameState::PAUSED:
		break;
	case GameState::TEAMCHANGE:
		ChangeTeam(_GD, _terrain, _context);
		break;
	case GameState::USINGWEAPON:
		UsingWeapon(_GD, _terrain, _context);
		break;
	case GameState::RESULTS:
		WinScreen(_GD);
		break;
	case GameState::SETUP:
		Setup(_GD, _terrain, _context);
		break;
	case GameState::SETUPSWAP:
		SetupSwapTeam(_GD, _terrain, _context);
		break;
	case GameState::RISING:
		break;
	}
}

//Game States
void LevelManager::Play(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	ManageObjects(_GD, _terrain, _context);
	m_teams[m_active].Control(_GD, m_d3d11device);
	if (Timer(_GD->m_dt, m_teams[m_active].Colour()))
	{
		m_timer = 3;
		m_state = GameState::TEAMCHANGE;
	}

	if (GameTimer(_GD->m_dt))
	{
		m_game_time = 30;
		m_state = GameState::RISING;
	}

	m_teams[m_active].Tick(_GD);
	if (m_teams[m_active].UseWeapon(_GD, m_objects, m_d3d11device))
	{
		m_timer = 5;
		m_state = GameState::USINGWEAPON;
	}
	m_teams[m_active].ChangeWormSprite(_GD, m_d3d11device);
	CheckTeamDeath();
}

void LevelManager::UsingWeapon(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	ManageObjects(_GD, _terrain, _context);
	if (!Timer(_GD->m_dt, m_teams[m_active].Colour()))
	{
		m_teams[m_active].Control(_GD, m_d3d11device);
	}
	else// if (m_teams[m_active].EndTurn())
	{
		m_state = GameState::TEAMCHANGE;
		m_timer = 3;
	}

	if (GameTimer(_GD->m_dt))
	{
		m_game_time = 30;
		m_state = GameState::RISING;
	}

	CheckTeamDeath();
}

void LevelManager::ChangeTeam(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	ManageObjects(_GD, _terrain, _context);
	//Dont change team if an object prevents it
	if (!m_continue)
	{
		GameTimer(_GD->m_dt);
		m_timer = 3;
		return;
	}

	if (CheckWin())
	{
		m_state = GameState::RESULTS;
		return;
	}

	if (Timer(_GD->m_dt, Color(Colors::LightSlateGray)))
	{
		CycleTeam();
		m_state = GameState::PLAYING;
	}
}

//Render
void LevelManager::RenderObjects(DrawData2D* _DD)
{
	for (auto obj : m_objects)
	{
		obj->Draw(_DD);
	}

	m_sea->Draw(_DD);

	for (auto &t : m_teams)
	{
		t.RenderHUD(_DD);
	}

	m_teams[m_active].RenderWormHUD(_DD);

	m_time_display->Draw(_DD);
	m_game_timer->Draw(_DD);

	//m_teams[m_active[0]].worms[m_active[1]]->DrawHUD(_DD);
}

void LevelManager::RenderDestruction(DrawData2D* _DD)
{
	for (auto mask : m_destruction)
	{
		mask->Draw(_DD);
	}
}


//Objects
void LevelManager::ManageObjects(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	_terrain->Map(_context);

	m_continue = true;

	for (auto obj : m_objects)
	{
		obj->Tick(_GD);
		obj->CheckHealth(_GD->m_dt);

		if (obj->GetCollider())
		{
			obj->GetCollider()->UpdateHitbox(obj->GetPos());
			ManageTerrainCollision(obj, _GD, _terrain, _context);
			TestCollisions(_GD, obj);
		}

		if (dynamic_cast<Weapon*>(obj))
		{
			if (dynamic_cast<Weapon*>(obj)->Spawn(_GD, m_objects, m_d3d11device))
			{
				break;
			}
		}

		if (TestWaterLevel(obj))
		{
			break;
		}

		//m_teams[m_active].TriggerEndTurn(obj->TriggerEndTurn());
		if (obj->StopTurnEnd())
		{
			m_continue = false;
		}

		if (obj->Explode().explode)
		{
			SpawnExplosion(obj);
			break;
		}

		if (obj->Delete())
		{
			DeleteObject(obj);
			break; //Break out of loop as data no longer exists
		}
	}

	_terrain->Unmap(_context);
}

void LevelManager::TestCollisions(GameData* _GD, GameObject2D* _object)
{
	auto coll = _object->GetCollider();

	for (auto other : m_objects)
	{
		auto other_coll = other->GetCollider();
		if (other == _object || !other_coll)
		{
			continue;
		}

		bool colliding = _object->IsCollided(other);
		bool collided = coll->Collided(other_coll->Hitbox());

		if (!colliding && collided)
		{
			_object->OnCollisionEnter(_GD, other);
		}
		else if (colliding && collided)
		{
			_object->OnCollision(_GD, other);
		}
		else if (colliding && !collided)
		{
			_object->OnCollisionExit(_GD, other);
		}

		//if ()
		//{ 
		//	/* Objects get stuck inside each other 
		//	if (_object->GetPhysComp())
		//	{
		//		_object->GetPhysComp()->ReactionForce(coll->CalculateNormal(other_coll->Hitbox()));
		//		//_object->GetPhysComp()->InAir(false);
		//	}
		//	*/
		//
		//}
	}
}

void LevelManager::ManageTerrainCollision(GameObject2D* _object, GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	auto coll = _object->GetCollider();
	auto phys = _object->GetPhysComp();

	std::array<int, 4> coll_data = coll->TerrainCollsionV(_terrain, _context, _GD, _object->GetPos());

	bool colliding = _object->IsCollided(m_stage);
	bool collided = coll_data != std::array<int, 4>{0, 0, 0, 0};

	if (!colliding && collided)
	{
		_object->OnCollisionEnter(_GD, m_stage);
	}
	else if (colliding && collided)
	{
		_object->OnCollision(_GD, m_stage);
	}
	else if (colliding && !collided)
	{
		_object->OnCollisionExit(_GD, m_stage);
	}

	if (phys)
	{
		//Calculate normal to collision
		if (collided)
		{
			//Move object if stuck in wall
			if (coll_data[0] > 0 && coll_data[1] > 0 && coll_data[2] > 0 && coll_data[3] > 0)
			{
				//phys->SetVelocityDir(coll->CalculateNormal(coll_data));
				phys->StuckInGround(coll->CalculateNormal(coll_data));
			}
			else //apply resistive forces
			{
				phys->ReactionForce(coll->CalculateNormal(coll_data));
			}
		}
		phys->ApplyGravity(coll_data[0] < 4);
		phys->ApplyVelocity(_GD->m_dt);
	}
}

void LevelManager::SpawnExplosion(GameObject2D* _object)
{
	Explosion* explosion = new Explosion(dynamic_cast<Worm*>(_object), m_d3d11device);
	explosion->SetData(_object->Explode());
	m_objects.push_back(explosion);
	m_destruction.push_back(new DestructionMask(m_d3d11device, explosion->GetPos(), explosion->GetScale()));

	if (dynamic_cast<Worm*>(_object))
	{
		dynamic_cast<Worm*>(_object)->StopExplosion();
	}

	DeleteObject(_object);
}

bool LevelManager::DeleteObject(GameObject2D* _obj)
{
	//Dont delete worms
	if (dynamic_cast<Worm*>(_obj))
	{
		dynamic_cast<Worm*>(_obj)->Kill(m_d3d11device);
		return false;
	}

	//auto it = std::find(m_objects.begin(), m_objects.end(), _obj);
	auto end = m_objects.end();
	auto result = std::remove(m_objects.begin(), end, _obj);

	if (result != end)
	{
		m_objects.erase(result, end);
		delete _obj;
		_obj = nullptr;
		return true;
	}
	return false;
}

Stage* LevelManager::GetStage()
{
	return m_stage;
}

void LevelManager::WinScreen(GameData* _GD)
{
	if (_GD->m_KBS_tracker.IsKeyPressed(Keyboard::Enter))
	{
		//EXIT BACK TO MENUS
	}
}

void LevelManager::CalculateLeaderboard()
{
	std::vector<int> scores;

	for (int i = 0; i < m_teams.size(); i++)
	{
		scores.push_back(m_teams[i].GetScore());
		m_leaderboard.push_back(i);
	}

	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		for (int i = 0; i < m_leaderboard.size() - 1; i++)
		{
			if (scores[i] < scores[i + 1])
			{
				int temp_score = scores[i];
				scores[i] = scores[i + 1];
				scores[i + 1] = temp_score;

				int temp_i = m_leaderboard[i];
				m_leaderboard[i] = m_leaderboard[i + 1];
				m_leaderboard[i + 1] = temp_i;
				swapped = true;
			}
		}
	}
}

void LevelManager::SetupWinScreen()
{
	TextGO2D* texta = new TextGO2D("GAME OVER");
	texta->SetPos(Vector2(500, 100));
	texta->SetScale(1.5f);
	m_objects.push_back(texta);
	
	TextGO2D* textb = new TextGO2D("ENTER TO CONTINUE");
	textb->SetPos(Vector2(450, 600));
	textb->SetScale(1.5f);
	m_objects.push_back(textb);

	for (int i=0; i<m_leaderboard.size(); i++)
	{
		int score = m_teams[m_leaderboard[i]].GetScore();
		TextGO2D* team_info = new TextGO2D(std::to_string(score));
		team_info->SetPos(Vector2(640, 500 - (20 * m_teams.size() * i)));
		team_info->SetColour(m_teams[m_leaderboard[i]].Colour());

		m_objects.push_back(team_info);
	}
}

void LevelManager::HudOcclusion()
{

	for (auto team : m_teams)
	{
		auto hud = team.HUD();
		auto worms = team.Worms();
		for (auto worm : worms)
		{
			hud->Occlusion(worm->GetCollider(), m_d3d11device);
		}
	}
}

void LevelManager::CycleTeam()
{
	m_teams[m_active].OnEndTurn(m_d3d11device);

	m_active++;
	for (int i = m_active; i < m_active + m_teams.size(); i++)
	{
		i %= m_teams.size();
		if (!m_teams[i].AllWormsDead())
		{
			m_active = i;
			m_timer = 20;
			m_teams[m_active].OnStartTrun();
			return;
		}
	}
}

bool LevelManager::Timer(float _gt, Color _col)
{
	m_timer -= _gt;

	if (m_timer < 10)
	{
		m_time_display->SetText("0" + std::to_string(int(m_timer)));
	}
	else
	{
		m_time_display->SetText(std::to_string(int(m_timer)));
	}
	
	m_time_display->SetColour(_col);

	if (m_timer <= 0)
	{
		m_timer = 0;
		return true;
	}
	return false; 
}

bool LevelManager::GameTimer(float _gt)
{
	m_game_time -= _gt;
	int minutes = int(m_game_time / 60.0f);
	int seconds = int(m_game_time) % 60;

	string min;
	string sec;

	if (minutes < 10)
	{
		min = "0";
	}
	min += std::to_string(minutes);

	if (seconds < 10)
	{
		sec = "0";
	}
	sec += std::to_string(seconds);

	m_game_timer->SetText(min + ":" + sec);

	if (m_game_time <= 0)
	{
		return true;
	}

	return false;
}

bool LevelManager::TestWaterLevel(GameObject2D* _object)
{
	if (_object->GetPhysComp() && _object->GetPos().y > m_water_height + 16)
	{
		_object->SetPos(Vector2(-500, m_water_height+16));
		return DeleteObject(_object);
	}

	return false;
}

bool LevelManager::CheckWin()
{
	int alive = m_teams.size();
	for (auto &team : m_teams)
	{
		if (team.AllWormsDead())
		{
			alive--;
		}
	}

	if (alive <= 1)
	{
		CalculateLeaderboard();
		SetupWinScreen();
		return true;
	}

	return false;
}

void LevelManager::CheckTeamDeath()
{
	for (int i=0; i<m_teams.size(); i++)
	{
		if (m_teams[i].SetScore(m_score))
		{
			m_score--;
		}
	}
}

void LevelManager::Setup(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	auto mouse = _GD->m_MS;
	auto worms = m_teams[m_active].Worms();

	_terrain->Map(_context);

	if (mouse.leftButton)
	{
		worms[m_worm_no]->SetPos(Vector2(mouse.x, mouse.y));
		worms[m_worm_no]->GetCollider()->UpdateHitbox(worms[m_worm_no]->GetPos());
		auto col = worms[m_worm_no]->GetCollider()->TerrainCollsionV(_terrain, _context, _GD, worms[m_worm_no]->GetPos());
		if (col == std::array<int, 4>{0, 0, 0, 0})
		{			
			m_timer = 1;
			m_state = GameState::SETUPSWAP;
		}
		else
		{
			worms[m_worm_no]->SetPos(Vector2(-500, 0));
		}
	}

	_terrain->Unmap(_context);

	ManageObjects(_GD, _terrain, _context);
}

void LevelManager::SetupSwapTeam(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	if (Timer(_GD->m_dt, Color(Colors::LightSlateGray)))
	{
		m_active++;
		if (m_active >= m_teams.size())
		{
			m_active %= m_teams.size();
			m_worm_no++;
			if (m_worm_no >= m_teams[0].Worms().size())
			{
				m_timer = 30;
				m_state = GameState::PLAYING;
				return;
			}
		}

		m_timer = 0;
		m_time_display->SetColour(m_teams[m_active].Colour());
		m_state = GameState::SETUP;
	}

	ManageObjects(_GD, _terrain, _context);
}
