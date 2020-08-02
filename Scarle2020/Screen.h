#pragma once
#include <d3d11_1.h>
#include "GameData.h"
#include "SimpleMath.h"
#include "RenderTarget.h"
#include "CommonStates.h"
#include "Stage.h"

struct GameData;
struct DrawData2D;

using namespace DirectX;
using namespace SimpleMath;

class Screen
{
public:
	Screen(ID3D11Device* _GD);
	~Screen() = default;

	virtual void Update(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context) = 0;
	virtual void Draw(DrawData2D* _DD, RenderTarget* _terrain, ID3D11DeviceContext* _context, CommonStates* _states) = 0;
	virtual Screen* Load() = 0;

	Color GetBG() { return m_bg; }

protected:
	Color m_bg = Colors::Black;
	ID3D11Device* m_d3d11device = nullptr;
	Stage* m_stage = nullptr;
};

