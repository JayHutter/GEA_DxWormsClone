#include "pch.h"
#include "Stage.h"


Stage::Stage(ID3D11Device* _GD, string _name) : ImageGO2D(_name, _GD)
{
	//StageObject* s = new StageObject("enclosure", _GD);
	//solids.push_back(s);
	m_origin = Vector2::Zero;
}	

Stage::~Stage()
{
	for (auto s : solids)
	{
		delete s;
		s = nullptr;
	}

	solids.clear();
}

void Stage::AddSolid(StageObject* _object)
{
	solids.push_back(_object);
}

void Stage::RenderSolids(DrawData2D* _DD)
{
	for (auto s : solids)
	{
		s->Draw(_DD);
	}
}

void Stage::Resize(float _scale)
{
	m_scale = Vector2(_scale, _scale);
	for (auto obj : solids)
	{
		obj->SetScale(_scale);
	}
}