#pragma once
#include "ImageGO2D.h"
#include "Worm.h"
#include "DestructionMask.h"

class Weapon :public ImageGO2D
{
public:
	Weapon(string _fileName, ID3D11Device* _GD) : ImageGO2D(_fileName, _GD) {}
	~Weapon() = default;
	Weapon(const Weapon& orig);

	virtual void Tick(GameData* _GD);
	virtual void OnCollision(GameData* _GD, GameObject2D* _other);
	virtual Weapon* Clone(ID3D11Device* _GD) = 0;

	virtual void Use(GameData* _GD, Worm* _owner) = 0;

	float Damage();
	Vector2 Knockback();
	bool EndTurn();

	DestructionMask* Destruction();

protected:
	float m_damage = 0;
	Vector2 m_knockback = Vector2::Zero;

	Worm* m_owner = nullptr;

	bool m_end = false;
	DestructionMask* m_destruction = nullptr;
};
