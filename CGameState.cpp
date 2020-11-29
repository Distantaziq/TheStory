#include"CGameState.h"

//#define DEBUG
#ifdef DEBUG
#  define DEBUG(x) { x }
#else
#  define DEBUG(x)
#endif

void CGameState::NextRoom(const Location& Location)
{
	if (IsLocationNew(Location))
	{
		AddToVisitedLocations(Location);
	}
	else
	{
		std::cout << "\nWe've been here before!";
	}

	UpdateCurrentLocation(Location);
	DEBUG(PrintLocation(GetCurrentLocation()); );

	bool IsChoiceValid{ false };
	do
	{
		PrintTextAndChoices(Location);

		if (IsExit(Location))
		{
			ActivateExit();
			break;
		}

		CHandleInput HandleInput;
		int userInput{ HandleInput.HandleNumberInput() };
		if (userInput > 0 && CountChoices(Location) > 0)
		{
			--userInput;
			if (userInput <= CountChoices(Location))
			{
				//std::cout << "You chose to " << Location.Choices[userInput].Text << std::endl;
				IsChoiceValid = true;
				SetNextLocation(FindLocationFromChoice(Location.Choices[userInput].LocationID));
				//Also post any triggered events that happen when we did this. Did a rock fall down?
			}
			else
			{
				system("cls");
				std::cout << "\nInvalid input, please try again!" << std::endl;
				continue;
			}
		}
		else
		{
			std::cerr << "The input is 0" << std::endl;
		}
	} while (!IsChoiceValid);
}

void CGameState::PlayerNameWrapperFunction()
{
	CPlayer& Player = CPlayer::AccessInstance();
	Player.HandlePlayerNaming();
}

void CGameState::RunGame()
{
	PlayerNameWrapperFunction();
	do
	{
		if (IsFirstRoom())
		{
			NextRoom(GetLocation(GetStartLocation()));
			IsNoLongerFirstRoom();
		}
		else
		{
			system("cls");
			NextRoom(GetNextLocation());
		}

	} while (!IsExitActivated());
	std::cout << "\nHope you enjoyed the adventure! Please come again!" << std::endl;
}


