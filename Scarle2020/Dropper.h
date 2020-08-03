#pragma once
#include "Weapon.h"
class Dropper : public Weapon
{
public:
	Dropper(float _damage, float _bounce, bool _scale_damage, float _explosion_rad, float _exp_damage, float _exp_knockback, float _time, string _sprite, ID3D11Device* _GD);
	~Dropper() = default;
	Dropper(const Dropper& _orig);

	virtual void Tick(GameData * _GD);

	virtual void OnCollisionEnter(GameData * _GD, GameObject2D * _other);
	virtual void OnCollision(GameData * _GD, GameObject2D * _other);
	virtual void OnCollisionExit(GameData * _GD, GameObject2D * _other);
	virtual Dropper* Clone(ID3D11Device * _GD);

	virtual void Aim(GameData * _GD);
	virtual void Use(GameData * _GD, Worm * _owner, float _charge);
	virtual bool Spawn(GameData * _GD, std::vector<GameObject2D*> & _objects, ID3D11Device * _DD);
private:
	float m_bounce = 0;
	bool m_scale_damage = false;
	float m_base_damage = 1;
};

