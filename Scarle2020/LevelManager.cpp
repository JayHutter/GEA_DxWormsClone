#include "pch.h"
#include "LevelManager.h"
#include "DrawData2D.h"
#include "GameData.h"

LevelManager::LevelManager()
{
	m_objects.reserve(100);
	m_destruction.reserve(100);
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

void LevelManager::Tick(GameData* _GD)
{
}

void LevelManager::SetupLevel(string _name, int _teams, ID3D11Device* _GD)
{
	for (int i=0; i<_teams; i++)
	{
		m_teams.push_back(Team(_GD, 4, default_colors[i], m_objects));
		//for (int j = 0; j < 4; j++)
		//{
		//	Worm* w = new Worm(_GD, m_teams.back().team_colour, "Worm " + std::to_string(j+1));
		//	w->SetColour(m_teams.back().team_colour);
		//	w->SetPos(Vector2(50 + (j * 50) + (300*i), 200)); //Test Tint : will use hud instead
		//	m_teams.back().worms.push_back(w);
		//	m_objects.push_back(w);	
		//}
	}

	m_stage = new Stage(_GD, _name);
}

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
	m_teams[m_active].GetWorm()->DrawName(_DD);
	//m_teams[m_active[0]].worms[m_active[1]]->DrawHUD(_DD);
}

void LevelManager::RenderDestruction(DrawData2D* _DD)
{
	for (auto mask : m_destruction)
	{
		mask->Draw(_DD);
	}
}

void LevelManager::Update(GameData* _GD, ID3D11Device* _DD)
{
	//ShowFrames(_GD->m_dt);
	m_teams[m_active].Tick(_GD);
	m_teams[m_active].AimWeapon(_GD);

	for (auto obj : m_objects)
	{
		obj->Tick(_GD);
		
		if (obj->GetCollider())
		{
			obj->GetCollider()->UpdateHitbox(obj->GetPos());
		}

		if (dynamic_cast<Weapon*>(obj))
		{
			if (dynamic_cast<Weapon*>(obj)->Spawn(_GD, m_objects, _DD))
			{
				return;
			}
		}

		if (obj->Explode().explode)
		{
			Explosion* explosion = new Explosion(dynamic_cast<Worm*>(obj), _DD);
			explosion->SetData(obj->Explode());
			m_objects.push_back(explosion);
			m_destruction.push_back(new DestructionMask(_DD, explosion->GetPos(), explosion->GetScale()));

			if (dynamic_cast<Worm*>(obj))
			{
				dynamic_cast<Worm*>(obj)->StopExplosion();
			}

			DeleteObject(obj);
			return;
		}

		if (obj->Delete())
		{
			DeleteObject(obj);
			return; //Break out of loop as data no longer exists
		}
	}
}

void LevelManager::UpdatePhysics(RenderTarget* _terrain, ID3D11DeviceContext* _context, GameData* _GD)
{
	_terrain->Map(_context);

	for (auto obj : m_objects)
	{
		PhysicsComp* phys = obj->GetPhysComp();
		CollisionComp* coll = obj->GetCollider();

		if (coll)
		{
			std::array<int, 4> coll_data = coll->TerrainCollsionV(_terrain, _context, _GD, obj->GetPos());

			bool colliding = obj->IsCollided(m_stage);
			bool collided = coll_data != std::array<int, 4>{0, 0, 0, 0};

			if (!colliding && collided)
			{
				obj->OnCollisionEnter(_GD, m_stage);
			}
			else if (colliding && collided)
			{
				obj->OnCollision(_GD, m_stage);
			}
			else if (colliding && !collided)
			{
				obj->OnCollisionExit(_GD, m_stage);
			}


			if (phys)
			{				
				//Calculate normal to collision
				if (collided)
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
				//phys->InAir(coll_data[0] < 4);
				//phys->ApplyGravity();
				phys->ApplyVelocity(_GD->m_dt);
			}		
		}
	}

	//DebugRender();

	_terrain->Unmap(_context);
}

void LevelManager::ManageCollisions(GameData* _GD)
{
	for (auto obj : m_objects)
	{
		auto coll = obj->GetCollider();
		if (!coll)
		{
			continue;
		}

		for (auto other : m_objects)
		{
			auto other_coll = other->GetCollider();
			if (other == obj || !other_coll)
			{
				continue;
			}
			
			bool colliding = obj->IsCollided(other);
			bool collided = coll->Collided(other_coll->Hitbox());

			if (!colliding && collided)
			{
				obj->OnCollisionEnter(_GD, other);
			}
			else if (colliding && collided)
			{
				obj->OnCollision(_GD, other);
			}
			else if (colliding && !collided)
			{
				obj->OnCollisionExit(_GD, other);
			}

			//if ()
			//{ 
			//	/* Objects get stuck inside each other 
			//	if (obj->GetPhysComp())
			//	{
			//		obj->GetPhysComp()->ReactionForce(coll->CalculateNormal(other_coll->Hitbox()));
			//		//obj->GetPhysComp()->InAir(false);
			//	}
			//	*/
			//
			//}
		}
	}
}

void LevelManager::DestroyStage(ID3D11Device* _DD, GameData* _GD)
{
	if (_GD->m_MS.rightButton)
	{
		m_destruction.push_back(new DestructionMask(_DD, Vector2(_GD->m_MS.x, _GD->m_MS.y), Vector2(1, 1)));
	}
}

Stage* LevelManager::GetStage()
{
	return m_stage;
}

//Refactor
void LevelManager::Input(GameData* _GD, ID3D11Device* _DD)
{
	auto key = _GD->m_KBS_tracker;
	auto worm = m_teams[m_active].GetWorm();

	if (worm->GetPhysComp()->AirTime() < 0.15f)
	{
		worm->Move(_GD->m_KBS.D + (_GD->m_KBS.A * -1));

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

	//if (_GD->m_MS.leftButton)
	if (key.IsKeyPressed(Keyboard::F))
	{
		m_teams[m_active].UseWeapon(_GD, m_objects, _DD);
	}
	
	//DEBUG : Worm swap
	if (key.IsKeyPressed(Keyboard::Tab))
	{
		m_teams[m_active].CycleWorm();
	}
	if (key.IsKeyPressed(Keyboard::LeftShift))
	{
		m_active++;
		m_active %= m_teams.size();
	}

	m_teams[m_active].CycleWeapon(key.IsKeyPressed(Keyboard::E) + (-1 * key.IsKeyPressed(Keyboard::Q)));

	//Test explosion
	if (key.IsKeyPressed(Keyboard::Enter))
	{
		m_teams[m_active].GetWorm()->TriggerExplosion();
	}
}

void LevelManager::DebugRender()
{	
	//Vector2 vel = m_teams[m_active[0]].worms[m_active[1]]->GetPhysComp()->GetVel();
	//
	//debug_text->SetText("[" + std::to_string(m_active[1]) + "] - " + std::to_string(vel.x) + ", " + std::to_string(vel.y));
	//debug_text->SetColour(m_teams[m_active[0]].team_colour);
}

void LevelManager::ShowFrames(float _gt)
{
	//frame_text->SetText(std::to_string(m_teams[m_active[0]].worms[m_active[1]]->GetPhysComp()->AirTime()));
}

void LevelManager::DeleteObject(GameObject2D* _obj)
{
	//Dont delete worms
	if (dynamic_cast<Worm*>(_obj))
	{
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