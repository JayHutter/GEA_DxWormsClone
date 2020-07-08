#include "pch.h"
#include "Stage.h"


Stage::Stage(ID3D11Device* _GD) : ImageGO2D("test_stage", _GD)
{
	m_pos = Vector2(0, 0);
}	