#include "CPlayer.h"

//#define DEBUG
#ifdef DEBUG
#  define DEBUG(x) { x }
#else
#  define DEBUG(x)
#endif

CPlayer& CPlayer::AccessInstance()
{
	static CPlayer Instance;
	return Instance;
}

void CPlayer::HandlePlayerNaming()
{
	std::cout << "\nGreetings! What is your name?" << std::endl;

	bool IsNameHandled{ false };

	do
	{
		std::string UserName{ "" };
		std::getline(std::cin, UserName, '\n');

		if (UserName == "")
		{
			std::cout << "Mysterious huh! Can I call you Snugglepuff then? My mother used to call me that.";

			std::cout << "\n[1] Yes"
				<< "\n[2] No" << std::endl;
			CHandleInput HandleNrInput;
			int UserInput{ HandleNrInput.HandleNumberInput() };
			bool IsValidInput{ false };
			do
			{
				if (UserInput == 1)
				{
					//SetPlayerName(GetPlaceholderName());
					std::cout << "Awesome! Let's continue!" << std::endl;
					IsNameHandled = true;
					IsValidInput = true;
				}
				else if (UserInput == 2)
				{
					system("cls");
					std::cout << "\nWhat would you like to be called, then?" << std::endl;
					IsValidInput = true;
					break;
				}
				else
				{
					std::cout << "I'm sorry, I didn't quite catch that?" << std::endl;
					continue;
				}
			} while (!IsValidInput);
		}
		else
		{
			AccessInstance().SetPlayerName(UserName);
			system("cls");
			std::cout << "\n" << UserName << ", huh. Nice name!";
			IsNameHandled = true;
		}
		DEBUG( std::cerr << "<<Current PlayerName = " << GetPlayerName(); );

	} while (!IsNameHandled);

}
