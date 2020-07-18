#pragma once
#include "ImageGO2D.h"
#include "StageObject.h"
#include <vector>

class Stage : public ImageGO2D
{
public:
	Stage(ID3D11Device* _GD, string _name);
	~Stage();

	void RenderSolids(DrawData2D* _DD);
private:
	std::vector<StageObject*> solids;
};

