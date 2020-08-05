#pragma once
#include "ImageGO2D.h"
#include "TextGO2D.h"

enum class Command
{
	NONE = 0,
	LEVELPLUS,
	LEVELMINUS,
	WORMPLUS,
	WORMMINUS,
	TEAMPLUS,
	TEAMMINUS,
	START
};

class Button : public ImageGO2D
{
public:
	Button(Command _command, Vector2 _pos, Vector2 _scale, string _text, ID3D11Device* _GD);
	~Button() = default;

	virtual Command Clicked(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);

	void SetState(bool _locked);
	void SetText(string _text);
protected:
	void ChangeColour(bool _lit);

	Command m_com = Command::NONE;
	RECT m_hitbox;
	TextGO2D* m_disp = nullptr;
	bool m_locked = false;

	const float m_delay = 0.2f;
	float m_time = m_delay;
};

