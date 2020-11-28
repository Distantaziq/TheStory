#pragma once
#include"CGameState.h"


struct SItem
{
	std::string ItemID{ "" };
	std::string ItemDescription{ "" };
};

struct SPlayer
{
	std::string PlayerName{ "" };
	int Health{ 100 };
	std::vector<SItem> Inventory;
};

class CPlayer
{
public: 

	static CPlayer& AccessInstance();

	SItem& dosomethingtoinventory() { return _PlayerData.Inventory[0]; };

	bool IsPlayerAlive() const { return _PlayerData.Health > 0 && _PlayerData.Health < 100; }
	void HandlePlayerNaming();
	void SetPlayerName(const std::string& NewName) { _PlayerData.PlayerName = NewName; }
	const SPlayer* GetPlayer() const { return _pPlayerPtr; }
	std::string GetPlayerName() const { return _pPlayerPtr->PlayerName; }

private:

	std::string DefaultName{ "Snugglepuff" };
	const int MAXHEALTH{ 100 };

	const SPlayer* const _pPlayerPtr{ &_PlayerData };			//should point to non-alterable _PlayerData
	SPlayer _PlayerData{ DefaultName, MAXHEALTH };


};

