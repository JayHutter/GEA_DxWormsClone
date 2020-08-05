#pragma once
#include "Weapon.h"

class Teleport : public Weapon
{
public:
	Teleport(ID3D11Device* _GD);
	~Teleport() = default;
	Teleport(const Teleport& _orig);

	virtual void Tick(GameData * _GD);

	virtual void OnCollisionEnter(GameData* _GD, GameObject2D* _other);
	virtual void OnCollision(GameData* _GD, GameObject2D* _other) {}
	virtual void OnCollisionExit(GameData* _GD, GameObject2D* _other);
	virtual Teleport* Clone(ID3D11Device * _GD);

	virtual void Aim(GameData* _GD) {}
	virtual void Use(GameData * _GD, Worm * _owner, float _charge);
	virtual bool Spawn(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD) { return false;  }
private:
	Vector2 m_target = Vector2::Zero;
	bool m_blocked = false;
};

