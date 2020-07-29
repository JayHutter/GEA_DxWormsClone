#include "pch.h"
#include "LevelManager.h"
#include "DrawData2D.h"
#include "GameData.h"

LevelManager::LevelManager(ID3D11Device* _GD)
{
	m_objects.reserve(100);
	m_destruction.reserve(100);

	m_d3d11device = _GD;
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
	m_time_display->SetPos(Vector2(300, 625));
	m_stage = new Stage(_GD, _name);
}


void LevelManager::Tick(GameData* _GD)
{
	for (auto team : m_teams)
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
		break;
	case GameState::SETUP:
		break;
	}
}

//Game States
void LevelManager::Play(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	ManageObjects(_GD, _terrain, _context);
	m_teams[m_active].Control(_GD, m_d3d11device);
	if (Timer(_GD->m_dt))
	{
		m_timer = 3;
		m_state = GameState::TEAMCHANGE;
	}

	m_teams[m_active].Tick(_GD);
	if (m_teams[m_active].UseWeapon(_GD, m_objects, m_d3d11device))
	{
		m_timer = 5;
		m_state = GameState::USINGWEAPON;
	}
	m_teams[m_active].ChangeWormSprite(_GD, m_d3d11device);
}

void LevelManager::UsingWeapon(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	ManageObjects(_GD, _terrain, _context);
	if (!Timer(_GD->m_dt))
	{
		m_teams[m_active].Control(_GD, m_d3d11device);
	}
	else// if (m_teams[m_active].EndTurn())
	{
		m_state = GameState::TEAMCHANGE;
		m_timer = 3;
	}
}

void LevelManager::ChangeTeam(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	ManageObjects(_GD, _terrain, _context);
	//Dont change team if an object prevents it
	if (!m_continue)
	{
		m_timer = 3;
		m_time_display->SetColour(Color(Colors::White));
		return;
	}

	if (Timer(_GD->m_dt))
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

	for (auto t : m_teams)
	{
		t.RenderHUD(_DD);
	}
	m_teams[m_active].RenderWormHUD(_DD);
	m_time_display->Draw(_DD);
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
				phys->SetVelocityDir(coll->CalculateNormal(coll_data));
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

void LevelManager::DeleteObject(GameObject2D* _obj)
{
	//Dont delete worms
	if (dynamic_cast<Worm*>(_obj))
	{
		dynamic_cast<Worm*>(_obj)->Kill(m_d3d11device);
		return;
	}

	//auto it = std::find(m_objects.begin(), m_objects.end(), _obj);
	auto end = m_objects.end();
	auto result = std::remove(m_objects.begin(), end, _obj);

	if (result != end)
	{
		m_objects.erase(result, end);
		delete _obj;
		_obj = nullptr;
	}
}

Stage* LevelManager::GetStage()
{
	return m_stage;
}

void LevelManager::WinCondition()
{
	std::vector<int> values;
	std::vector<int> indices;

	for (int i=0; i<m_teams.size(); i++)
	{
		values.push_back(m_teams[i].Health());
		indices.push_back(i);
	}

	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		for (int i = 0; i < values.size() -1; i++)
		{
			if (values[i] < values[i + 1])
			{
				int temp_val = values[i];
				int temp_ind = indices[i];

				values[i] = values[i + 1];
				indices[i] = indices[i + 1];
				
				values[i + 1] = temp_val;
				indices[i + 1] = temp_ind;
				swapped = true;
			}
		}
	}

	for (int i = 0; i < indices.size(); i++)
	{
		m_teams[indices[i]].SetPlacing(i+1);
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
	m_active %= m_teams.size();

	if (m_teams[m_active].Health() <= 0)
	{
		CycleTeam();
	}

	m_timer = 20;
	m_teams[m_active].OnStartTrun();
	//On start
}

bool LevelManager::Timer(float _gt)
{
	m_timer -= _gt;
	m_time_display->SetText(std::to_string(int(m_timer)));
	m_time_display->SetColour(m_teams[m_active].Colour());

	if (m_timer <= 0)
	{
		m_timer = 0;
		return true;
	}
	return false; 
}