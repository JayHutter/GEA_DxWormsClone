#pragma once
#include "Screen.h"
#include "Button.h"
#include "InputBox.h"
#include "Stage.h"
#include "TextGO2D.h"

class Menu : public Screen
{
public:
	Menu(ID3D11Device* _GD);
	~Menu() = default;

	virtual void Update(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context);

	void SetupMenu();
	virtual void Draw(DrawData2D* _DD, RenderTarget* _terrain, ID3D11DeviceContext* _context, CommonStates* _states);

	virtual Screen* Load();

private:
	void TestButtons(GameData* _GD);
	void GetLevels();


	void ChangeLevel(int _dir);
	void ChangeWormCount(int _dir);
	void ChangeTeamCount(int _dir);
	void PlayGame();

	void LoadStage();

	std::vector<Button*> m_buttons;
	std::vector<void (Menu::*)()> m_button_functions;
	std::vector<GameObject2D*> m_objects;
	std::vector<string> m_levels;

	InputBox* m_worms = nullptr;
	InputBox* m_teams = nullptr;
	Button* m_play = nullptr;
	TextGO2D* m_name = nullptr;

	int m_selection = 0;
	bool m_start = false;
};

