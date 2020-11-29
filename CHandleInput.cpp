#include "CHandleInput.h"

//#define DEBUG
#ifdef DEBUG
#  define DEBUG(x) { x }
#else
#  define DEBUG(x)
#endif

std::string CHandleInput::ProcessInput(const std::string& ToInt)
{
	std::string Ret{};
	for (int i{ 0 }; i < ToInt.length(); ++i)
	{
		//Since the digits 0-9 come after each other in the ASCII table
		if (ToInt[i] >= '0' && ToInt[i] <= '9')
		{
			Ret += ToInt[i];
		}
	}
	if (Ret == "")
	{
		Ret = '0';
	}
	return Ret;
}

const int CHandleInput::HandleNumberInput()
{
	std::cin.clear();
	std::string UserInput{ "" };
	std::string HandledInput{};
	do
	{
		std::getline(std::cin, UserInput, '\n');
		HandledInput = ProcessInput(UserInput);
		if (HandledInput == "0")
		{
			std::cout << "Please enter a number!" << std::endl;
		}

	} while (HandledInput == "0");

		DEBUG(std::cerr << "\n<<HandledInput: " << HandledInput << std::endl;);
	const int Ret{ std::stoi(HandledInput) };
	std::endl(std::cout);

	return Ret;
}
