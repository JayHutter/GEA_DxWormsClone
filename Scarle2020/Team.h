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
	void SelectWeapon(GameData* _GD, std::vector<GameObject2D*>& _objects);
	void DeleteWorm(Worm* _worm);
	void AimWeapon(GameData* _GD);

	void RenderHUD(DrawData2D* _DD);

private:
	//Team Info
	Color m_colour = Colors::Red;

	//Worms
	std::vector<Worm*>  m_worms;
	int m_current = 0;

	//Weapons
	int m_selection = 0;
	int weapon_count = 0;
	std::vector<Weapon*> m_weapons; //All available weapons to the team - Clone to spawn
	std::vector<int> m_available; //-1 is infinite
	Weapon* m_selected_weapon = nullptr;
	bool m_charging = false;
};

