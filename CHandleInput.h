#pragma once
#include"CGameData.h"

class CGameData;

class CHandleInput: public CGameData
{
public:
	std::string ProcessInput(const std::string& ToInt);
	const int HandleNumberInput();


};

