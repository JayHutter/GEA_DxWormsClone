#include "pch.h"
#include "Menu.h"
#include "GameData.h"
#include "DrawData2D.h"
#include "LevelManager.h"
#include "nlohmann/json.hpp"
#include <fstream>

Menu::Menu(ID3D11Device* _GD) : Screen(_GD)
{
}

void Menu::SetupMenu()
{
	Button* b1 = new Button(Command::LEVELMINUS, Vector2(50, 575), Vector2(1, 1), "<", m_d3d11device);
	Button* b2 = new Button(Command::LEVELPLUS, Vector2(800, 575), Vector2(1, 1), ">", m_d3d11device);
	Button* b4 = new Button(Command::TEAMPLUS, Vector2(1200, 125), Vector2(0.5, 1),"+", m_d3d11device);
	Button* b5 = new Button(Command::TEAMMINUS, Vector2(970, 125), Vector2(0.5, 1), "-", m_d3d11device);
	Button* b6 = new Button(Command::WORMPLUS, Vector2(1200, 375), Vector2(0.5, 1), "+", m_d3d11device);
	Button* b7 = new Button(Command::WORMMINUS, Vector2(970, 375), Vector2(0.5, 1), "-", m_d3d11device);
	m_play = new Button(Command::START, Vector2(970, 575), Vector2(2.3, 1), "PLAY", m_d3d11device);
	m_teams = new InputBox(Command::NONE, Vector2(1053, 125), Vector2(1, 1), 4, 2, 8, m_d3d11device);
	m_worms = new InputBox(Command::NONE, Vector2(1053, 375), Vector2(1, 1), 8, 1, 10, m_d3d11device);

	m_buttons.push_back(b1);
	m_buttons.push_back(b2);
	m_buttons.push_back(b4);
	m_buttons.push_back(b5);
	m_buttons.push_back(b6);
	m_buttons.push_back(b7);
	m_buttons.push_back(m_play);
	m_buttons.push_back(m_teams);
	m_buttons.push_back(m_worms);

	GetLevels();

	m_name = new TextGO2D(m_levels[0]);
	m_name->SetPos(Vector2(300, 600));

	TextGO2D* w = new TextGO2D("WORMS");
	w->SetPos(Vector2(1030, 300));
	TextGO2D* t = new TextGO2D("PLAYERS");
	t->SetPos(Vector2(1030, 50));

	m_objects.push_back(w);
	m_objects.push_back(t);
	m_objects.push_back(m_name);

	m_stage = new Stage(m_d3d11device, "");
	LoadStage();
}

void Menu::GetLevels()
{
	std::ifstream file("levels/stages.json");
	if (!file.is_open())
	{
		m_play->SetState(true);
		return;
	}
	m_play->SetState(false);
	nlohmann::json level;
	file >> level;
	file.close();

	auto levels = level["stages"];
	for (auto l : levels)
	{
		m_levels.push_back(l["name"].get<string>());
	}


}

void Menu::Update(GameData* _GD, RenderTarget* _terrain, ID3D11DeviceContext* _context)
{
	TestButtons(_GD);
	QuitGame(_GD);
}

void Menu::Draw(DrawData2D* _DD, RenderTarget* _terrain, ID3D11DeviceContext* _context, CommonStates* _states)
{
	_DD->m_Sprites->Begin(SpriteSortMode_Deferred, _states->NonPremultiplied());
	
	for (auto b : m_buttons)
	{
		b->Draw(_DD);
	}

	for (auto o : m_objects)
	{
		o->Draw(_DD);
	}

	if (m_stage)
	{
		m_stage->Draw(_DD);
	}

	_DD->m_Sprites->End();
}

void Menu::TestButtons(GameData* _GD)
{
	for (auto b : m_buttons)
	{
		Command com = b->Clicked(_GD);

		switch (com)
		{
		case Command::NONE:
			break;
		case Command::LEVELPLUS:
			ChangeLevel(1);
			break;
		case Command::LEVELMINUS:
			ChangeLevel(-1);
			break;
		case Command::TEAMPLUS:
			ChangeTeamCount(1);
			break;
		case Command::TEAMMINUS:
			ChangeTeamCount(-1);
			break;
		case Command::WORMPLUS:
			ChangeWormCount(1);
			break;
		case Command::WORMMINUS:
			ChangeWormCount(-1);
			break;
		case Command::START:
			PlayGame();
			break;
		}
	}
}

void Menu::ChangeLevel(int _dir)
{
	m_selection += _dir;

	if (m_selection < 0)
	{
		m_selection += m_levels.size();
	}

	m_selection %= m_levels.size();

	LoadStage();
}

void Menu::ChangeWormCount(int _dir)
{
	int val = m_worms->GetValue();
	val += _dir;

	m_worms->SetValue(val);
}

void Menu::ChangeTeamCount(int _dir)
{
	int val = m_teams->GetValue();
	val += _dir;

	m_teams->SetValue(val);
}

void Menu::PlayGame()
{
	m_start = true;
}

void Menu::LoadStage()
{
	delete m_stage;
	m_stage = nullptr;

	m_name->SetText(m_levels[m_selection]);

	std::ifstream file("levels/" + m_levels[m_selection] + ".json");
	if (!file.is_open())
	{
		m_play->SetState(true);
		return;
	}
	m_play->SetState(false);
	nlohmann::json level;
	file >> level;
	file.close();

	m_stage = new Stage(m_d3d11device, m_levels[m_selection]);

	auto objects = level["objects"];
	for (auto& obj : objects)
	{
		StageObject* stg_obj = new StageObject(obj["sprite"].get<string>(), m_d3d11device);
		stg_obj->SetPos(Vector2(obj["xPos"].get<float>(), obj["yPos"].get<float>()));
		m_stage->AddSolid(stg_obj);
	}

	m_stage->Resize(0.75f);
}

Screen* Menu::Load()
{
	if (m_start)
	{
		m_start = false;
		auto level = new LevelManager(m_d3d11device);

		int worm = m_worms->GetValue();
		int team = m_teams->GetValue();
		level->SetupLevel(m_levels[m_selection],team,worm );

		return level;
	}

	return nullptr;
}