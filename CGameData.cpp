#include"CGameData.h"

//#define DEBUG
#ifdef DEBUG
#  define DEBUG(x) { x } //using { x } makes it do whatever you put in DEBUG(put cerr or a function here)
#else
#  define DEBUG(x)
#endif
/*
LocationActions::LocationActions(const std::string& anAction, const std::string actionText)
{
	ActionID = anAction;
	Text = actionText;
}
*/
//What we choose to do in the location
LocationChoices::LocationChoices(const std::string& NextID, const std::string& aText)
{
	LocationID = NextID;
	Text = aText;
}

//The location, also has a vector for LocationChoices called Choices
Location::Location(const std::string& AnID, const std::string& aText)
{
	ID = AnID;
	Text = aText;
}

CGameData::CGameData()
{
	CreateLocations();
		DEBUG( PrintLocations(_SLocations); );
		DEBUG( DebugLocations(); );
}

void CGameData::DebugLocations()
{
	int DuplicateCount{ 0 };
	for (size_t i{ 0 }; i < _SLocations.size(); ++i)
	{
		for (size_t j{ i + 1 }; j < (_SLocations.size() - 1); ++j)					
		{
			if (_SLocations[i].ID == _SLocations[j].ID)
			{
				++DuplicateCount;
				std::cerr << "<<ID [ " << _SLocations[i].ID << " ] has a duplicate Location ID" << std::endl;
			}
		}
	}
	std::endl(std::cerr);
	std::cerr << "<<Amount of duplicate Location IDs found: " << DuplicateCount << std::endl;


	for (size_t i{ 0 }; i < _SLocations.size(); ++i)
	{
		if (_SLocations[i].Choices.size() == 0)
		{
			std::cerr << "<<Location [ " << _SLocations[i].ID << " ] has no Choices" << std::endl;
		}
		else
		{
			for (size_t j{ 0 }; j < _SLocations[i].Choices.size(); ++j)
			{
				if (ChoicesLocationExistsWithID(_SLocations[i].Choices[j].LocationID))
				{
					break;
				}
				else
				{
					std::cerr << "<<Location [" << _SLocations[i].ID << "] has a Choice with an invalid Location"
						<< "\n Choices LocationID [ " << _SLocations[i].Choices[j].LocationID << " ] is not a valid Location" << std::endl;
				}
				std::endl(std::cerr);
			}
		}
	}
	std::endl(std::cerr);
}

void CGameData::PrintLocation(const Location& Location) const
{
	std::cout << "\nLocation ID: " << Location.ID
		<< "\n  Text: " << Location.Text
		<< "\nLocation Choices: ";

	if (Location.Choices.size() == 0)
	{
		std::cerr << "\n  No choices for this location" << std::endl;
	}
	else
	{
		for (size_t i{ 0 }; i < Location.Choices.size(); ++i)
		{
			std::cout << "\n  ID: " << Location.Choices[i].LocationID << "\n  Text: " << Location.Choices[i].Text;
		}
	}
}

void CGameData::PrintLocations(const std::vector<Location>& LocationStorage) const
{
	for (size_t i{ 0 }; i < LocationStorage.size(); ++i)
	{
		std::cout << "\nLocation " << i << "\n  ID: " << LocationStorage[i].ID << "\n  Text: " << LocationStorage[i].Text << "\nLocation Choices ";

		if (LocationStorage[i].Choices.size() == 0)
		{
			std::cerr << "\n  No choices for this location" << std::endl;
		}
		else
		{
			for (size_t j{ 0 }; j < LocationStorage[i].Choices.size(); ++j)
			{
				std::cout << "\n  ID: " << LocationStorage[i].Choices[j].LocationID << "\n  Text: " << LocationStorage[i].Choices[j].Text;
			}
		}
		std::endl(std::cout);
	}
	std::endl(std::cout);
}

bool CGameData::ChoicesLocationExistsWithID(const std::string& NextID)
{
	for (size_t i{ 0 }; i < _SLocations.size(); ++i)
	{
		if (NextID == _SLocations[i].ID)
		{
			return true;
		}
	}
	std::endl(std::cout);
	
	return false;
}

void CGameData::CreateLocations()
{
	Location room1(_STARTLOCATION, "You are in the first room of this story.");
	room1.Choices.push_back(LocationChoices("room2", "Go north"));
	room1.Choices.push_back(LocationChoices("exit", "Exit"));
	_SLocations.push_back(room1);

	Location room2("room2", "You are in the second room of this story.");
	room2.Choices.push_back(LocationChoices("room3", "Continue ahead"));
	room2.Choices.push_back(LocationChoices("exit", "Exit"));
	_SLocations.push_back(room2);

	Location room3("room3", "You are in the third room of this story. There's a nice waterfall here.");
	room3.Choices.push_back(LocationChoices("room2", "Go back"));
	room3.Choices.push_back(LocationChoices("look", "Look at waterfall"));
	room3.Choices.push_back(LocationChoices("exit", "Exit"));
	//room3.Actions.push_back(LocationChoices("look", "Look at waterfall"));
	_SLocations.push_back(room3);

	Location look("look", "Upon closer inspection, it seems that the waterfall has a mysterious indigo color.What could have caused the discoloring?");
	look.Choices.push_back(LocationChoices("room3", "Return"));
	_SLocations.push_back(look);

	Location exit(_EXITLOCATION, "This is the exit! :o");
	_SLocations.push_back(exit);

	UpdateCurrentLocation(room1);
}

void CGameData::PrintChoices(const Location& Location)
{
	for (size_t i{ 0 }; i < Location.Choices.size(); ++i)
	{
		std::cout << "\n [" << i + 1 << "] " << Location.Choices[i].Text;
	}
}

void CGameData::PrintTextAndChoices(const Location& Location)
{
	if (Location.Text.size() != 0)
	{
		std::cout << "\n" << Location.Text;
		PrintChoices(Location);
		std::endl(std::cout);
	}
	else
	{
		std::cerr << "Invalid Location, no Location Text to print" << std::endl;
	}
	if (Location.ID != _EXITLOCATION)
	{
		std::cout << "\nWhat would you like to do next?" << std::endl;
	}
}

int CGameData::CountChoices(const Location& Location) const
{
	int AmountOfChoices{ 0 };
	for (size_t i{ 0 }; i < Location.Choices.size(); ++i)
	{
		++AmountOfChoices;
	}

	if (AmountOfChoices == 0)
	{
		std::cerr << "Returning zero choices" << std::endl;
	}

	return AmountOfChoices;
}

void CGameData::AddToVisitedLocations(const Location& Location)
{
	_SVisitedLocations.push_back(Location);
}

bool CGameData::IsLocationNew(const Location& Location) const
{
	if (_SVisitedLocations.size() != 0)
	{
		for (size_t i{ 0 }; i < _SVisitedLocations.size(); ++i)
		{
			if (_SVisitedLocations[i].ID == Location.ID)
			{
				return false;
			}
		}
	}
	else
	{
		return true;
	}
}

Location CGameData::FindLocationFromChoice(const std::string& Locator) const
{
	for (size_t i{ 0 }; i < _SLocations.size(); ++i)
	{
		if (_SLocations[i].ID == Locator)
		{
			return _SLocations[i];
		}
	}
	std::cerr << "FindLocation could not find the LocationID" << std::endl;
}

Location CGameData::GetLocation(const std::string& Locator) const
{
	if (Locator != "")
	{
		for (size_t i{ 0 }; i < _SLocations.size(); ++i)
		{
			if (_SLocations[i].ID == Locator)
			{
				return _SLocations[i];
			}
		}
	}
	else
	{
		std::cerr << "Locator is empty, Location does not exist" << std::endl;
	}
}

bool CGameData::IsExit(const Location& Location) const
{
	if (Location.ID == _EXITLOCATION)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CGameData::IsStart(const Location& Location) const
{
	if (Location.ID == _STARTLOCATION)
	{
		return true;
	}
	else
	{
		return false;
	}
}



