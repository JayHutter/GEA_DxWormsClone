#pragma once
#include "Weapon.h"

class Grenade : public Weapon
{
public:
	Grenade(float _radius, float _bounce, float _time, float _damage, float _knockback, int _frags, string _sprite, ID3D11Device* _GD);
	~Grenade() = default;
	Grenade(const Grenade& _orig);

	virtual void Tick(GameData * _GD);

	virtual void OnCollisionEnter(GameData* _GD, GameObject2D* _other);
	virtual void OnCollision(GameData* _GD, GameObject2D* _other);
	virtual void OnCollisionExit(GameData* _GD, GameObject2D* _other);
	virtual Grenade* Clone(ID3D11Device * _GD);

	virtual void Aim(GameData * _GD);
	virtual void Use(GameData * _GD, Worm * _owner, float _charge);
	virtual bool Spawn(GameData * _GD, std::vector<GameObject2D*> & _objects, ID3D11Device * _DD);
private:
	int m_frags = 0;
	
	enum class State
	{
		NEUTRAL = 0,
		FRAGMENT, 
		EXPLODE
	};

	State m_state = State::NEUTRAL;

	float m_bounce = 0;
};

