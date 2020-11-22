#include"CGameState.h"

//#define DEBUG
#ifdef DEBUG
#  define DEBUG(x) { x }
#else
#  define DEBUG(x)
#endif

void CGameState::NextRoom(const Location& Location)
{
	bool IsChoiceValid{ false };
	do
	{
		UpdateCurrentLocation(Location);
		DEBUG( PrintLocation(GetCurrentLocation()); );
		if (IsLocationNew(Location))
		{
			AddToVisitedLocations(Location);
		}
		else
		{
			std::cout << "\nWe've been here before!";
		}
		PrintTextAndChoices(Location);

		if (IsExit(Location))
		{
			ActivateExit();
			break;
		}

		CHandleInput HandleInput;
		int userInput{ HandleInput.HandleInput() };
		
		if (userInput > 0)
		{
			--userInput;
		}
		else
		{
			std::cerr << "The input is 0" << std::endl;
		}

		if (CountChoices(Location) > 0)
		{
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
	} while (!IsChoiceValid);
}

void CGameState::HandleRooms(/*const Location& Location*/)
{
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

}

void CGameState::StartGame()
{
	std::cout << "\nWelcome to The Story! Please enjoy your stay!" << std::endl;
	HandleRooms();
	std::cout << "\nHope you enjoyed the adventure! Please come again!" << std::endl;
}

