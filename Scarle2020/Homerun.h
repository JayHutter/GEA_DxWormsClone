#pragma once
#include "Weapon.h"
#include "GameData.h"

class Homerun : public Weapon
{
public:
	Homerun(ID3D11Device* _GD);
	~Homerun() = default;

	virtual void Tick(GameData* _GD);
	virtual void OnCollision(GameData* _GD, GameObject2D* _other);

	virtual void Use(GameData* _GD, Worm* _owner);

private:
	float m_active = 0.2f;
	float m_time = 0;
};

