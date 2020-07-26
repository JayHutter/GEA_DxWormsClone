#include "pch.h"
#include "MissileLauncher.h"
#include "GameData.h"
#include "Projectile.h"

MissileLauncher::MissileLauncher(ID3D11Device* _GD) : Weapon("MLauncher", _GD)
{
	//FOR DEBUG WHILE NO SPRITE
	m_colour = Colors::Red;
}

MissileLauncher::MissileLauncher(const MissileLauncher& _orig) : Weapon(_orig)
{

}

void MissileLauncher::Tick(GameData* _GD)
{
	m_time += _GD->m_dt;
	
	if (m_time > m_active)
	{
		m_delete = true;
		m_end = true;
	}
}

MissileLauncher* MissileLauncher::Clone(ID3D11Device* _GD)
{
	MissileLauncher* clone = new MissileLauncher(*this);
	clone->SetSprite(m_filename, _GD);
	return clone;
}

void MissileLauncher::Aim(GameData* _GD)
{

}

void MissileLauncher::Use(GameData* _GD, Worm* _owner)
{
	m_owner = _owner;
}

bool MissileLauncher::Spawn(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD)
{
	if (m_fired)
	{
		return false;
	}

	Projectile* missile = new Projectile("Missile", 16, 16, false, true, 30, Vector2(200, 0), 1, 100, m_owner, _DD);
	//missile->SetPos(m_pos + Vector2(50, -50));
	missile->SetPos(Vector2(700, 200));
	_objects.push_back(missile);
	//missile->GetPhysComp()->AddForce(Vector2(200, -500));

	m_fired = true;

	return true;
}