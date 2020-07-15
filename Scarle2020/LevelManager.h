#pragma once
#include <d3d11_1.h>
#include <vector>
#include <list>
#include "CommonStates.h"
#include "Worm.h"
#include "Stage.h"
#include "GameObject2D.h"
#include "ImageGO2D.h"


struct Team
{
	std::vector<Worm*> worms;
	Color team_colour = Colors::Red;
};

class LevelManager
{
public:
	LevelManager() = default;
	~LevelManager();

	void SetupLevel(string _name, int _teams, ID3D11Device* _GD);

	void RenderObjects(DrawData2D* _DD);
	void Update(GameData* _GD);
	void UpdatePhysics(RenderTarget* _terrain, ID3D11DeviceContext* _context, GameData* _GD);
	void Input(GameData* _GD);

	Stage* GetStage();

private:
	std::vector<Team> m_teams;
	std::vector<GameObject2D*> m_objects;

	Stage* m_stage = nullptr;
	Color default_colors[4] = { Colors::Red, Colors::Blue, Colors::Green, Colors::Yellow };
	int m_active[2] = { 0, 3 }; //{Team, Worm}
};

