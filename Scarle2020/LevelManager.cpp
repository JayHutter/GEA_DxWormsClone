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
				phys->ApplyGravity(!coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos()));

				//if (!coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos()))
				//{
				//	phys->ApplyGravity(true);
				//}
				//else
				//{
				//	phys->ApplyGravity(false);
				//	float xpos = obj->GetPos().x;
				//	obj->SetPos(Vector2(xpos, 50));
				//}
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