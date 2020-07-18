#include "pch.h"
#include "DestructionMask.h"

DestructionMask::DestructionMask(ID3D11Device* _GD, Vector2 _pos, Vector2 _scale) : ImageGO2D("destruction_mask", _GD)
{
	m_pos = _pos;
	m_scale = _scale;
}