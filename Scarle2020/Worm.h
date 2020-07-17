#pragma once

#include <d3d11_1.h>
#include <stdio.h>
#include "ImageGO2D.h"
#include "PhysicsComp.h"

class Worm : public ImageGO2D
{
public:
	Worm(ID3D11Device* _GD);
	~Worm() = default;

	virtual void Tick(GameData* _GD);
	void Move(float _dir);


private:
};