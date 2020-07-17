#include "pch.h"
#include "Worm.h"

Worm::Worm(ID3D11Device* _GD) : ImageGO2D("worm", _GD)
{
	c_phys = new PhysicsComp(&m_pos);
	c_collider = new CollisionComp(20, 24);
}

void Worm::Tick(GameData* _GD)
{
	
}

void Worm::Move(float _dir)
{

}