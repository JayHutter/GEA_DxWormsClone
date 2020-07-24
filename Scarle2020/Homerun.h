#pragma once
#include "Weapon.h"
#include "GameData.h"

class Homerun : public Weapon
{
public:
	Homerun(ID3D11Device* _GD);
	~Homerun() = default;
	Homerun(const Homerun& orig);

	virtual void Tick(GameData* _GD);

	virtual void OnCollisionEnter(GameData* _GD, GameObject2D* _other);
	virtual void OnCollision(GameData* _GD, GameObject2D* _other);
	virtual void OnCollisionExit(GameData* _GD, GameObject2D* _other);

	virtual Homerun* Clone(ID3D11Device* _GD) override;

	virtual void Use(GameData* _GD, Worm* _owner);

private:
	const float m_active = 0.05f;
	float m_time = 0;
};

