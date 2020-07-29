#pragma once
#include "ImageGO2D.h"
#include "Worm.h"
#include "DestructionMask.h"

class Weapon :public ImageGO2D
{
public:
	Weapon(string _fileName, ID3D11Device* _GD) : ImageGO2D(_fileName, _GD) {}
	~Weapon() = default;
	Weapon(const Weapon& _orig);

	virtual void Tick(GameData* _GD);

	virtual void OnCollisionEnter(GameData* _GD, GameObject2D* _other) {}
	virtual void OnCollision(GameData* _GD, GameObject2D* _other) {}
	virtual void OnCollisionExit(GameData* _GD, GameObject2D* _other) {}
	virtual Weapon* Clone(ID3D11Device* _GD) = 0;

	void SetOwner(Worm* _owner);
	virtual void Aim(GameData* _GD) = 0;
	virtual void Use(GameData* _GD, Worm* _owner, float _charge) = 0;
	virtual bool Spawn(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD) = 0;

	bool Chargeable();

	bool EndTurn();

	DestructionMask* Destruction();

	string AimSprite();

protected:
	float m_active = 1;
	float m_time = 0;

	float m_damage = 0;
	Vector2 m_knockback = Vector2::Zero;
	
	bool m_chargeable = false;
	float m_charge = 0;

	Worm* m_owner = nullptr;

	DestructionMask* m_destruction = nullptr;
	string m_aim_sprite = "Worm";
};

