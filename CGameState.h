#pragma once
#include"CGameData.h"
#include"CHandleInput.h"

class CGameData;

class CGameState: public CGameData
{
public:
	void NextRoom(const Location& Location);
	void HandleRooms(/*const Location& Location*/);
	void StartGame();



	void ActivateExit() { _IsExitActivated = true; }
	bool IsExitActivated() const { return _IsExitActivated; }
	bool IsFirstRoom() const { return _IsFirstRoom; }
	void IsNoLongerFirstRoom() { _IsFirstRoom = false; }

private:
	bool _IsFirstRoom{ true };
	bool _IsExitActivated{ false };

};

