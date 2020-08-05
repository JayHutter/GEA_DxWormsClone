#pragma once
#include "ImageGO2D.h"
class StageObject : public ImageGO2D
{
public:
	StageObject(string _sprite, ID3D11Device* _DD);
	~StageObject() = default;
private:

};

