#pragma once

#include <d3d11_1.h>
#include <stdio.h>
#include "ImageGO2D.h"
#include "TextGO2D.h"
#include "PhysicsComp.h"

class Worm : public ImageGO2D
{
public:
	Worm(ID3D11Device* _GD, Color _colour, string _name);
	~Worm() = default;

	virtual void Tick(GameData* _GD);
	virtual void OnCollisionEnter(GameData* _GD, GameObject2D* _other) {}
	virtual void OnCollision(GameData* _GD, GameObject2D* _other) {}
	virtual void OnCollisionExit(GameData* _GD, GameObject2D* _other) {}
	
	void Move(float _dir);

	void DrawHUD(DrawData2D* _DD);
	void DrawHealth(DrawData2D* _DD);
	void DrawName(DrawData2D* _DD);
	//Test Explosion - Objects will delete when they explode so this is unneccesarry 
	void TriggerExplosion() { m_explode = ExplosionData{true, 35, 1, m_pos, 300}; }
	void StopExplosion() { m_explode.explode = false; }

private:
	void UpdateHUD();

	bool m_move = true;
	string m_name = "Worm";
	TextGO2D* m_health_display = nullptr;
	TextGO2D* m_name_display = nullptr;
	Color m_colour = Colors::Red;
};