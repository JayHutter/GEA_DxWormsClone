#pragma once
#include "Weapon.h"
class Mine : public Weapon
{
public:
	Mine(int _fake_chance, ID3D11Device* _GD);
	~Mine() = default;
	Mine(const Mine& _orig);

	virtual void Tick(GameData * _GD);

	virtual void OnCollisionEnter(GameData * _GD, GameObject2D * _other);
	virtual void OnCollision(GameData * _GD, GameObject2D * _other);
	virtual void OnCollisionExit(GameData * _GD, GameObject2D * _other);
	virtual Mine* Clone(ID3D11Device * _GD);

	virtual void Aim(GameData * _GD);
	virtual void Use(GameData * _GD, Worm * _owner, float _charge);
	virtual bool Spawn(GameData * _GD, std::vector<GameObject2D*> & _objects, ID3D11Device * _DD);
private:
	bool m_triggered = false;
	bool m_fake = false;
};

