#ifndef _GAME_OBJECT_2D_H_
#define _GAME_OBJECT_2D_H_

//=================================================================
//the base Game Object 2D
//=================================================================

#include <d3d11_1.h>
#include "SimpleMath.h"
#include <string>
#include "PhysicsComp.h"
#include "CollisionComp.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace std;

struct GameData;
struct DrawData2D;

struct ExplosionData
{
	bool explode = false;
	float damage = 0;
	float scale = 1;
	Vector2 pos = Vector2::Zero;
	float knockback = 0;
};

class GameObject2D
{
public:
	GameObject2D();
	virtual ~GameObject2D() {};

	virtual void Tick(GameData* _GD) = 0;
	virtual void Draw(DrawData2D* _DD) = 0;

	//setters

	void SetPos(Vector2 _pos) { m_pos = _pos; }
	void SetRot(float _rot) { m_rotation = _rot; }
	void SetColour(Color _colour) { m_colour = _colour; }
	void SetScale(Vector2 _scale) { m_scale = _scale; }
	void SetScale(float _scale) { m_scale = _scale * Vector2::One; }
	void SetOrigin(Vector2 _origin) { m_origin = _origin; }

	bool Delete() { return m_delete; }
	PhysicsComp* GetPhysComp();
	Vector2 GetPos() { return m_pos; }
	CollisionComp* GetCollider() { return c_collider; }
	Vector2 GetScale() { return m_scale; }
	int Direction() { return m_dir; }
	ExplosionData Explode() { return m_explode; }
	bool IsCollided(GameObject2D* _obj);
	void CheckHealth(float _gt);
	int Health() { return m_health; }

	void AddHealth(int _health);

	virtual void OnCollisionEnter(GameData* _GD, GameObject2D* _other) = 0;
	virtual void OnCollision(GameData* _GD, GameObject2D* _other) = 0;
	virtual void OnCollisionExit(GameData* _GD, GameObject2D* _other) = 0;
	bool TriggerEndTurn() { return m_end; }
	
protected:
	void RemoveFromCollided(GameObject2D* _obj);
	void TriggerDeath(float _gt);

	Vector2 m_pos;
	float m_rotation;
	Vector2 m_scale;
	Color m_colour;
	Vector2 m_origin;

	int m_dir = 1;

	PhysicsComp *c_phys = nullptr;
	CollisionComp* c_collider = nullptr; 
	bool m_delete = false;
	int m_health = 100;
	ExplosionData m_explode;
	bool m_invincible = true;

	float m_death_timer = 0;
	bool m_end = false;
	std::vector<GameObject2D*> m_collided;
};

#endif