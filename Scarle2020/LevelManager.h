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

	void SetupLevel(string _name, int _teams, ID3D11Device* _GD);

	void Tick(GameData* _GD);
	void Update(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);

	void RenderObjects(DrawData2D* _DD);
	void RenderDestruction(DrawData2D* _DD);

	void ManageObjects(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);

	Stage* GetStage();

private:
	void Play(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);
	void UsingWeapon(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);
	void ChangeTeam(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);

	void TestCollisions(GameData* _GD, GameObject2D* _object);
	void ManageTerrainCollision(GameObject2D* _object, GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);
	void SpawnExplosion(GameObject2D* _object);
	void DeleteObject(GameObject2D* _obj);
	void HudOcclusion();
	void CycleTeam();
	bool Timer(float _gt, Color _col);
	bool GameTimer(float _gt);

	void WinCondition();

	Stage* m_stage = nullptr;

	std::vector<Team> m_teams;
	std::vector<GameObject2D*> m_objects;
	std::vector<DestructionMask*> m_destruction;

	Color default_colors[4] = { Colors::Red, Colors::Blue, Colors::Green, Colors::Orange };
	int m_active = 0;

	ID3D11Device* m_d3d11device = nullptr;
	TextGO2D* m_time_display = nullptr;
	TextGO2D* m_game_timer = nullptr;

	float m_timer = 0;
	float m_game_time = 900;
	bool m_continue = true;

	enum class GameState
	{
		PLAYING = 0,
		PAUSED, 
		TEAMCHANGE,
		USINGWEAPON,
		RESULTS,
		SETUP,
		RISING
	};

	GameState m_state = GameState::PLAYING;
};

