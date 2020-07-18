#pragma once
#include <d3d11_1.h>
#include "Worm.h"
#include <vector>

class Team
{
public:
	Team(ID3D11Device* _GD, int _worms, Color _colour, std::vector<GameObject2D*> &_objects);
	~Team();

	Worm* GetWorm();
	void CycleWorm();

	void RenderHUD(DrawData2D* _DD);

private:
	std::vector<Worm*>  m_worms;

	int m_current = 0;
	Color m_colour = Colors::Red;
};

