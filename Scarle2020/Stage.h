#pragma once
#include "ImageGO2D.h"
#include "StageObject.h"
#include <vector>

class Stage : public ImageGO2D
{
public:
	Stage(ID3D11Device* _GD, string _name);
	~Stage();
	void AddSolid(StageObject* _object);

	void RenderSolids(DrawData2D* _DD);
	void Resize(float _scale);
private:
	std::vector<StageObject*> solids;
};

