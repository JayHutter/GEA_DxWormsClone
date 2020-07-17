#pragma once
#include <d3d11_1.h>
#include "ImageGO2D.h"

 class DestructionMask : public ImageGO2D
{
public:
	 DestructionMask(ID3D11Device* _GD, Vector2 _pos);
	 ~DestructionMask() = default;
private:

};

