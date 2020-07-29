#pragma once
#include "ImageGO2D.h"
class Healthbar : public ImageGO2D
{
public:
	Healthbar(int _maximum, Color _colour, Vector2 _pos, ID3D11Device* _GD);
	~Healthbar() = default;

	virtual void Draw(DrawData2D* _DD);

	void SetHealth(int _health);
	void SetPercent(float _percent);

private:
	float m_max = 100;
	float m_health = 0;
	float m_percent = 1;

	ImageGO2D* m_meter = nullptr;
};

