#pragma once
#include "ImageGO2D.h"
#include "Worm.h"

class Projectile : public ImageGO2D
{
public:
	Projectile(string _sprite, float _width, float _height, bool _bounce, bool _gravity, float _damage, Vector2 _knockback, float _explosion_rad, float _time, Worm* _owner, ID3D11Device* _DD);
	~Projectile() = default;

	virtual void Tick(GameData* _GD);

	virtual void OnCollisionEnter(GameData* _GD, GameObject2D* _other);
	virtual void OnCollision(GameData* _GD, GameObject2D* _other);
	virtual void OnCollisionExit(GameData* _GD, GameObject2D* _other);

private:
	Worm* m_owner = nullptr;
	float  m_explosion = 0;
	Vector2 m_knockback = Vector2::Zero;
	float m_damage = 0;

	float m_time = 0;
	float m_active = 1000;
};

