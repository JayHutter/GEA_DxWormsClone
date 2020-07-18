#include "pch.h"
#include "Team.h"

Team::Team(ID3D11Device* _GD, int _worms, Color _colour, std::vector<GameObject2D*> &_objects)
{
	for (int i = 0; i < _worms; i++)
	{
		Worm* worm = new Worm(_GD, _colour, "Worm " + std::to_string(i));
		m_worms.push_back(worm);
		_objects.push_back(worm);
		
		worm->SetPos(Vector2(50 + (i * 100), 300));
	}

	m_colour = _colour;
}

Team::~Team()
{
	for (auto w : m_worms)
	{
		w = nullptr; // Worm also in object stack and is deleted from there
	}

	m_worms.clear();
}

Worm* Team::GetWorm()
{
	return m_worms[m_current];
}

void Team::CycleWorm()
{
	m_current++;
	m_current %= m_worms.size();
}