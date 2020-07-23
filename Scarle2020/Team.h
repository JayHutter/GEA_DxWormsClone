#pragma once
#include <d3d11_1.h>
#include <vector>
#include "Worm.h"
#include "Weapon.h"
#include "AllWeapons.h"


class Team
{
public:
	Team(ID3D11Device* _GD, int _worms, Color _colour, std::vector<GameObject2D*> &_objects);
	~Team();

	virtual void Tick(GameData* _GD);

	Worm* GetWorm();
	void CycleWorm();

	//Weapons
	void SetupWeapons(ID3D11Device* _GD);
	void CycleWeapon(int _dir);
	void UseWeapon(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD);
	void TrackWeapon(GameData* _GD, std::vector<GameObject2D*>& _objects);
	void DeleteWorm(Worm* _worm);

	void RenderHUD(DrawData2D* _DD);

private:
	//Team Info
	Color m_colour = Colors::Red;

	//Worms
	std::vector<Worm*>  m_worms;
	int m_current = 0;

	//Weapons
	int m_selection = 0;
	static const int weapon_count = 1;
	std::array<Weapon*, weapon_count> m_weapons; //All available weapons to the team - Clone to spawn
	std::array<int, weapon_count> m_available; //-1 is infinite
};

