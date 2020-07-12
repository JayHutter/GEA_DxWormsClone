#include "pch.h"
#include "Worm.h"

Worm::Worm(ID3D11Device* _GD) : ImageGO2D("worm", _GD)
{
	c_phys = new PhysicsComp(&m_pos);
	c_collider = new CollisionComp(32, 32);
}

void Worm::Tick(GameData* _GD)
{
	
}