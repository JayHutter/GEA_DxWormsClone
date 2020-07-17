#include "pch.h"
#include "DestructionMask.h"

DestructionMask::DestructionMask(ID3D11Device* _GD, Vector2 _pos) : ImageGO2D("alpha_test", _GD)
{
	m_pos = _pos;
}