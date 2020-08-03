#pragma once
#include "Button.h"

class InputBox : public Button
{
public:
	InputBox(Command _command, Vector2 _pos, Vector2 _scale, int _default, int _min, int _max, ID3D11Device* _GD);
	~InputBox() = default;

	virtual Command Clicked(GameData* _GD) override;

	void SetValue(int _val);
	int GetValue() { return m_value; }
private:
	void GetInputValue(GameData* _GD);
	void NormaliseValue();

	int m_value = 0;
	int m_min = 1;
	int m_max = 10;
};

