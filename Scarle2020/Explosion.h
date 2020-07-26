#pragma once
#include "ImageGO2D.h"
#include "Worm.h"
#include "DestructionMask.h"

class Explosion : public ImageGO2D
{
public:
	Explosion(Worm* _owner, ID3D11Device* _GD);
	~Explosion() = default;

	void SetData(float _size, Vector2 _pos, float damage, float _knockback);
	void SetData(ExplosionData _data);

	virtual void Tick(GameData* _GD);
	virtual void OnCollisionEnter(GameData* _GD, GameObject2D* _other);
	virtual void OnCollision(GameData* _GD, GameObject2D* _other);
	virtual void OnCollisionExit(GameData* _GD, GameObject2D* _other);
private:
	void Damage(GameObject2D* _other);

	Worm* m_owner = nullptr;
	float m_damage = 0;
	float m_knockback = 0;
	float m_time = 0;
	const float m_exist = 0.2f;
};

