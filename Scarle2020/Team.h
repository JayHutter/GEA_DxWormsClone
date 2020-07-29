#pragma once
#include <d3d11_1.h>
#include <vector>
#include "Worm.h"
#include "Weapon.h"
#include "AllWeapons.h"
#include "Healthbar.h"

class Team
{
public:
	Team(ID3D11Device* _GD, int _worms, Color _colour, int _port, std::vector<GameObject2D*>& _objects);
	~Team();

	virtual void Tick(GameData* _GD);
	void Update(GameData* _GD);

	Worm* GetWorm();
	void CycleWorm();

	//Weapons
	void SetupWeapons(ID3D11Device* _GD);
	void CycleWeapon(int _dir);
	void UseWeapon(GameData* _GD, std::vector<GameObject2D*>& _objects, ID3D11Device* _DD);
	void SelectWeapon(GameData* _GD, std::vector<GameObject2D*>& _objects);
	void DeleteWorm(Worm* _worm);
	void AimWeapon(GameData* _GD);
	void OnEndTurn(ID3D11Device* _DD);
	void OnStartTrun();

	void RenderHUD(DrawData2D* _DD);
	void ChangeWormSprite(GameData* _GD, ID3D11Device* _DD);

	void SetPlacing(int _placing);
	int Health() { return m_total_health; }
	std::vector<Worm*> Worms() { return m_worms; }
	Healthbar* HUD() { return m_hud; }
	bool EndTurn() { return m_end; }
	void TriggerEndTurn(bool _end);
	Color Colour() { return m_colour; }

private:
	void UpdateHealth();

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
	bool m_can_attack = true;
	float m_charge = 0;

	int m_total_health = 0;
	int m_placing = 0;
	Healthbar* m_hud = nullptr;
	bool m_end = false;
};

