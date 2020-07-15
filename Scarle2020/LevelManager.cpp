#include "pch.h"
#include "LevelManager.h"
#include "DrawData2D.h"
#include "GameData.h"

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

	m_stage = new Stage(_GD);
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
				//coll->UpdateHitbox(obj->GetPos());
				//This works but is messy 
				//Test below and to side they are moving
				//if (phys->MovingRight() && coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos(), Side::Negative, Side::Positive))
				//{
				//	phys->MultiplyVelocity(Vector2(-0.5, 0));
				//}
				//if (phys->MovingRight() && coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos(), Side::Center, Side::Positive))
				//{
				//	phys->MultiplyVelocity(Vector2(0.5, 0));
				//	phys->AddForce(Vector2(0, -15));
				//
				//}
				//else if (phys->MovingRight() && coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos(),Side::Positive, Side::Positive))
				//{
				//	phys->AddForce(Vector2(0, -8));
				//}
				//
				//if (phys->MovingLeft() && coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos(), Side::Negative, Side::Negative))
				//{
				//	phys->MultiplyVelocity(Vector2(-0.5, 0));
				//}
				//else if (phys->MovingLeft() && coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos(), Side::Center, Side::Negative))
				//{
				//	phys->MultiplyVelocity(Vector2(0.5, 0));
				//	phys->AddForce(Vector2(0, -15));
				//}
				//else if (phys->MovingLeft() && coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos(), Side::Positive, Side::Negative))
				//{
				//	phys->AddForce(Vector2(0, -8));
				//}

				//phys->ApplyGravity(!coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos(), Side::Positive, Side::Center));
				std::array<int, 4> coll_data = coll->TerrainCollsionV(_terrain, _context, _GD, obj->GetPos());				
				phys->ApplyGravity(coll_data[0] == 0);
				
				for (auto val : coll_data)
				{
					if (val > 0)
					{
						std::array<int, 4> debug = coll_data;
					}
				}	

				//Calculate normal to collision
				Vector2 coll_vect = Vector2::Zero;

				if (coll_data[3] > coll_data[2])
				{
					coll_vect.x = coll_data[3] * -1;
				}
				else if (coll_data[2] > coll_data[3])
				{
					coll_vect.x = coll_data[2];
				}

				if (coll_data[1] > coll_data[0])
				{
					coll_vect.y = coll_data[1];
				}
				else if (coll_data[0] > coll_data[1])
				{
					coll_vect.y = coll_data[0] * -1;
				}

				if (coll_vect != Vector2::Zero)
				{
					//auto debug = coll_data;
					//auto debug1 = coll_vect;
					phys->SetVelocity(coll_vect); //Set as test
				}

				
			}
			phys->ApplyVelocity(_GD->m_dt);
		}
	}

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

	if (_GD->m_KBS.D)
	{
		worm->GetPhysComp()->SetVelocityX(20);
	}
	else if (_GD->m_KBS.A)
	{
		worm->GetPhysComp()->SetVelocityX(-20);
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