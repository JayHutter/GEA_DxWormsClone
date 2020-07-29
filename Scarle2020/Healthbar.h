#pragma once
#include "ImageGO2D.h"
#include "TextGO2D.h"

class Healthbar : public ImageGO2D
{
public:
	Healthbar(int _maximum, Color _colour, Vector2 _pos, ID3D11Device* _GD);
	~Healthbar() = default;

	virtual void Draw(DrawData2D* _DD);

	void SetHealth(int _health);
	void SetPercent(float _percent);
	void SetPosition(int _pos);

private:
	void UpdatePositions();

	float m_max = 100;
	float m_health = 0;
	float m_percent = 1;

	ImageGO2D* m_meter = nullptr;
	TextGO2D* m_text = nullptr;
	int m_position = 0;
	Vector2 m_region = Vector2::Zero;
};

