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
	void Move(float _dir);

	void DrawHUD(DrawData2D* _DD);
	void DrawHealth(DrawData2D* _DD);
	void DrawName(DrawData2D* _DD);

private:
	void UpdateHUD();

	bool m_move = true;
	string m_name = "Worm";
	int m_health = 100;
	TextGO2D* m_health_display = nullptr;
	TextGO2D* m_name_display = nullptr;
	Color m_colour = Colors::Red;
};