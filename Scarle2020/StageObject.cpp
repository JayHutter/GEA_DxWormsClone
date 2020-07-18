#include "pch.h"
#include "StageObject.h"

StageObject::StageObject(string _sprite, ID3D11Device* _DD) : ImageGO2D(_sprite, _DD)
{
	m_origin = Vector2::Zero;
}
