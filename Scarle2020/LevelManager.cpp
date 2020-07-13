#include "pch.h"
#include "LevelManager.h"
#include "DrawData2D.h"
#include "GameData.h"

void LevelManager::SetupLevel(string _name, int _teams, ID3D11Device* _GD)
{
	for (int i=0; i<_teams; i++)
	{
		m_teams.push_back(new Team());
		for (int j = 0; j < 4; j++)
		{
			Worm* w = new Worm(_GD);
			w->SetPos(Vector2(100 + (j * 50) + (200*i), 200 + (50 * i)));
			m_teams.back()->worms.push_back(w);
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
	for (auto obj : m_objects)
	{
		PhysicsComp* phys = obj->GetPhysComp();
		CollisionComp* coll = obj->GetCollider();

		if (phys)
		{
			if (coll)
			{
				phys->ApplyGravity(!coll->TerrainCollision(_terrain, _context, _GD, obj->GetPos()));
			}

			phys->ApplyVelocity(_GD->m_dt);
		}
	}
}

Stage* LevelManager::GetStage()
{
	return m_stage;
}