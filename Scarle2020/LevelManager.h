#pragma once
#include "Screen.h"
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

class LevelManager : public Screen
{
public:
	LevelManager(ID3D11Device* _GD);
	~LevelManager();

	void SetupLevel(string _name, int _teams, ID3D11Device* _GD);
	void SetupLevel(string _name, int _teams, int _worms);

	void Tick(GameData* _GD);
	virtual void Update(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);
	virtual Screen* Load() { return nullptr; }

	virtual void Draw(DrawData2D* _DD, RenderTarget* _terrain, ID3D11DeviceContext* _context, CommonStates* _states);
	void RenderObjects(DrawData2D* _DD);
	void RenderDestruction(DrawData2D* _DD);

	void ManageObjects(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);

private:
	void Play(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);
	void UsingWeapon(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);
	void ChangeTeam(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);

	void TestCollisions(GameData* _GD, GameObject2D* _object);
	void ManageTerrainCollision(GameObject2D* _object, GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);
	void SpawnExplosion(GameObject2D* _object);
	bool DeleteObject(GameObject2D* _obj);
	void HudOcclusion();
	void CycleTeam();
	bool Timer(float _gt, Color _col);
	void GameTimer(float _gt);
	bool TestWaterLevel(GameObject2D* _object);
	bool CheckWin();
	void WinScreen(GameData* _GD);
	void CalculateLeaderboard();
	void CheckTeamDeath();
	void SetupWinScreen();
	void Setup(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);
	void SetupSwapTeam(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);
	void Rising(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);

	std::vector<Team> m_teams;
	std::vector<GameObject2D*> m_objects;
	std::vector<DestructionMask*> m_destruction;
	std::vector<int> m_leaderboard;
	int m_score = 0;

	Color default_colors[4] = { Colors::Red, Colors::Blue, Colors::Green, Colors::Orange };
	int m_active = 0;

	TextGO2D* m_time_display = nullptr;
	TextGO2D* m_game_timer = nullptr;
	ImageGO2D* m_sea = nullptr;

	float m_timer = 0;
	float m_game_time = 900;
	bool m_continue = true;
	float m_water_height = 600;
	int m_worm_no = 0;
	bool m_sudden_death = false;

	enum class GameState
	{
		PLAYING = 0,
		PAUSED, 
		TEAMCHANGE,
		USINGWEAPON,
		RESULTS,
		SETUP,
		SETUPSWAP,
		RISING
	};

	GameState m_state = GameState::SETUP;
};

