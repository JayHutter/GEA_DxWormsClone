#pragma once
#include <d3d11_1.h>
#include <vector>
#include <list>
#include "CommonStates.h"
#include "Worm.h"
#include "Stage.h"
#include "GameObject2D.h"
#include "ImageGO2D.h"
#include "TextGO2D.h"
#include "DestructionMask.h"
#include "Team.h"
#include "Explosion.h"


class LevelManager
{
public:
	LevelManager(ID3D11Device* _GD);
	~LevelManager();

	virtual void Tick(GameData* _GD);

	void SetupLevel(string _name, int _teams, ID3D11Device* _GD);

	void RenderObjects(DrawData2D* _DD);
	void RenderDestruction(DrawData2D* _DD);
	void Update(GameData* _GD, ID3D11Device* _DD);
	void UpdatePhysics(RenderTarget* _terrain, ID3D11DeviceContext* _context, GameData* _GD);
	void ManageCollisions(GameData* _GD);
	void DestroyStage(ID3D11Device* _DD, GameData* _GD);
	void Input(GameData* _GD, ID3D11Device* _DD);

	Stage* GetStage();

private:
	void DebugRender();
	void ShowFrames(float _gt);
	void DeleteObject(GameObject2D* _obj);

	Stage* m_stage = nullptr;

	std::vector<Team> m_teams;
	std::vector<GameObject2D*> m_objects;
	std::vector<DestructionMask*> m_destruction;

	Color default_colors[4] = { Colors::Red, Colors::Blue, Colors::Green, Colors::Orange };
	int m_active = 0;

	TextGO2D *debug_text = new TextGO2D("NO WORM SELECT");
	TextGO2D *frame_text = new TextGO2D("0");	

	ID3D11Device* m_d3d11device = nullptr;
};

