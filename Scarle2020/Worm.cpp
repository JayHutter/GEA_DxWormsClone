#include "Worm.h"

Worm::Worm(ID3D11Device* _GD) : ImageGO2D("worm", _GD)
{
	c_phys = new PhysicsComp(&m_pos);
	c_collider = new CollisionComp(4, 4);
}

void Worm::Tick(GameData* _GD)
{
	
}