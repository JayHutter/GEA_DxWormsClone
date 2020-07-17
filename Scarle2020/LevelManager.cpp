#include "pch.h"
#include "LevelManager.h"
#include "DrawData2D.h"
#include "GameData.h"

LevelManager::LevelManager()
{
	frame_text->SetPos(Vector2(1000, 100));
	frame_text->SetColour(Color(Colors::HotPink));

	m_objects.push_back(debug_text);
	m_objects.push_back(frame_text);
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

	for (auto t : m_teams)
	{
		t.worms.clear();
	}
	
	m_teams.clear();
}

void LevelManager::SetupLevel(string _name, int _teams, ID3D11Device* _GD)
{
	for (int i=0; i<_teams; i++)
	{
		m_teams.push_back(Team());
		m_teams.back().team_colour = default_colors[i];
		for (int j = 0; j < 4; j++)
		{
			Worm* w = new Worm(_GD);
			w->SetColour(m_teams.back().team_colour);
			w->SetPos(Vector2(50 + (j * 50) + (300*i), 200)); //Test Tint : will use hud instead
			m_teams.back().worms.push_back(w);
			m_objects.push_back(w);	
		}
	}

	m_stage = new Stage(_GD, _name);
}

void LevelManager::RenderObjects(DrawData2D* _DD)
{
	for (auto obj : m_objects)
	{
		obj->Draw(_DD);
	}
}

void LevelManager::Update(GameData* _GD)
{
	ShowFrames(_GD->m_dt);
}

void LevelManager::UpdatePhysics(RenderTarget* _terrain, ID3D11DeviceContext* _context, GameData* _GD)
{
	_terrain->Map(_context);

	for (auto obj : m_objects)
	{
		PhysicsComp* phys = obj->GetPhysComp();
		CollisionComp* coll = obj->GetCollider();

		if (phys)
		{
			if (coll)
			{				
				std::array<int, 4> coll_data = coll->TerrainCollsionV(_terrain, _context, _GD, obj->GetPos());				

				//Calculate normal to collision
				if (coll_data != std::array<int, 4>{0, 0, 0, 0})
				{
					//Move object if stuck in wall
					if (coll_data[0] > 0 && coll_data[1] > 0 && coll_data[2] > 0 && coll_data[3] > 0)
					{		
						phys->SetVelocity(coll->CalculateNormal(coll_data));
					}
					else //apply resistive forces
					{
						phys->ReactionForce(coll->CalculateNormal(coll_data));
					}
				}	
				phys->ApplyGravity(coll_data[0] < 4); 
			}
			phys->ApplyVelocity(_GD->m_dt);
		}
	}

	DebugRender();

	_terrain->Unmap(_context);
}

Stage* LevelManager::GetStage()
{
	return m_stage;
}

void LevelManager::Input(GameData* _GD)
{
	auto key = _GD->m_KBS_tracker;
	auto worm = m_teams[m_active[0]].worms[m_active[1]];

	if (worm->GetPhysComp()->AirTime() < 0.15f)
	{
		if (_GD->m_KBS.D)
		{
			//worm->GetPhysComp()->AddForce(Vector2(11, 0));
			worm->GetPhysComp()->SetVelocityX(50);
		}
		else if (_GD->m_KBS.A)
		{
			//worm->GetPhysComp()->AddForce(Vector2(-11, 0));
			worm->GetPhysComp()->SetVelocityX(-50);
		}

		if (key.IsKeyReleased(Keyboard::Space))
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
	
	//DEBUG : Worm swap
	if (key.IsKeyPressed(Keyboard::Tab))
	{
		if (m_active[1] == 3)
		{
			m_active[0] += 1;
			m_active[0] %= 4;
		}
		m_active[1] += 1;
		m_active[1] %= 4;
	}
}

void LevelManager::DebugRender()
{	
	Vector2 vel = m_teams[m_active[0]].worms[m_active[1]]->GetPhysComp()->GetVel();

	debug_text->SetText("[" + std::to_string(m_active[1]) + "] - " + std::to_string(vel.x) + ", " + std::to_string(vel.y));
	debug_text->SetColour(m_teams[m_active[0]].team_colour);
}

void LevelManager::ShowFrames(float _gt)
{
	frame_text->SetText(std::to_string(m_teams[m_active[0]].worms[m_active[1]]->GetPhysComp()->AirTime()));
}
