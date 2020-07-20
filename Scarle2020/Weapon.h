#pragma once
#include "ImageGO2D.h"

class Weapon :public ImageGO2D
{
public:
	Weapon() = default;
	~Weapon() = default;

	void Tick(GameData* _GD);
	
	void Use(GameData* _GD, Worm* _owner);
	void Collided(bool _collided, GameObject2D* _other);
	float Damage();
	Vector2 Knockback();
	bool EndTurn();

	DestructionMask* Destruction();

private:
	bool m_end = false;
	DestructionMask* m_destruction = nullptr;
	float m_damage = 0;
	Vector2 m_knocback = Vector2::Zero;
	Worm* m_owner = nullptr;
};

