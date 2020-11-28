#include"CGameData.h"
#include"CPlayer.h"

//ways to die? get points for what items you collected?
//write commands on certain prompts? "what do you do?" "look"
//save game & load game

//if the player location is nullptr, end game

//have multiple start locations, pick a start location at random


//#define DEBUG
#ifdef DEBUG
#  define DEBUG(x) { x } //using { x } makes it do whatever you put in DEBUG(put cerr or a function here)
#else
#  define DEBUG(x)
#endif

//#define FILEDEBUG
#ifdef FILEDEBUG
#  define FILEDEBUG(x) { x }
#else
#  define FILEDEBUG(x)
#endif

//#define STRINGDEBUG
#ifdef STRINGDEBUG
#  define STRINGDEBUG(x) { x }
#else
#  define STRINGDEBUG(x)
#endif



/*
CReader& CReader::AccessInstance()
{
	static CReader Instance;
	//Instance.IsDirectoryEmpty();
	return Instance;
}
*/

void CGameData::ReadContentDirectory()
{
	HANDLE hFindFile;
	WIN32_FIND_DATA FindFileData;
	int AmountOfLocations{ 0 };
	//LARGE_INTEGER szDir;
	//We need to track amount of l Location files we have so we can loop through them and push_back content into _SLocations

	hFindFile = FindFirstFile(AccessContentFolder(), &FindFileData);
	//If FindFirstFile fails, the return value is INVALID_HANDLE_VALUE
	if (INVALID_HANDLE_VALUE == hFindFile)
	{
			FILEDEBUG( std::cerr << "\n<<Error in finding file"
			<< "\n Error - " << GetLastError() << std::endl; );
		//std::cerr << "\nInvalid File Handle Value " << std::endl;
	}
	else
	{
			FILEDEBUG( std::cout << "\n<<File found!" << std::endl; );
		do
		{
			//What is the file name? //needs to be wcout since it returns wide characters
			FILEDEBUG( std::wcout << "<<File name - " << FindFileData.cFileName << std::endl; );
			std::wstring temp{ FindFileData.cFileName };
			if (temp.at(0) == _FileLocationPrefix)
			{
				_LocationFileNames.push_back(temp);
			}
			++AmountOfLocations;

		} while (FindNextFile(hFindFile, &FindFileData) != 0);
			FILEDEBUG(std::cerr << "<<Amount of files found: " << AmountOfLocations << std::endl; );
		UpdateLocationAmount(AmountOfLocations);
			FILEDEBUG( DebugPrintStoredFileNames(); );
	}
	FindClose(hFindFile);
	ReadFileContent();
}

void CGameData::DebugPrintStoredFileNames()
{
	std::endl(std::cerr);
	for (auto& i : _LocationFileNames)
	{
		std::wcout << "<<Location file name: " << i << std::endl;
	}
}

int CGameData::DetermineAmountOfLines(const std::string& FileName)
{
	std::string temp{ "" };
	int AmountOfLines{ 0 };

	std::ifstream fFile(FileName);
	while (std::getline(fFile, temp))
	{
		++AmountOfLines;
	}
		FILEDEBUG(std::cerr << "<<Amount of lines in Content file >> " << FileName << " << are: " << AmountOfLines << std::endl; );
	return AmountOfLines;
}

std::string CGameData::ConvertWString(const std::wstring& ConvertThis)
{
	std::string One{ _ContentDirPrefix };
	std::string Two{ ConvertThis.begin(), ConvertThis.end() };
	std::string Ret{ One + Two };
		FILEDEBUG(std::cerr << "<<Current converted string: " << Ret << std::endl;);
	return Ret;
}

void CGameData::FormatStrings(std::string& FirstString, std::string& SecondString)
{
	std::string Delimiter{ GetStringDelimiter() };

	size_t Position{ 0 };
	std::string tempString{ FirstString };
	while ((Position = FirstString.find(Delimiter)) != std::string::npos)
	{
		SecondString = tempString.substr((Position + 1), (tempString.length() - Position));
		FirstString.erase(Position, SecondString.length());
	}
}

std::string CGameData::FormatString(std::string& FileString, const std::string& StartDelimiter, const std::string& EndDelimiter)
{
	size_t FirstDelimiterPosition{ FileString.find(StartDelimiter) };
	size_t StartOfSubstring{ FirstDelimiterPosition + StartDelimiter.length() };
	size_t LastDelimiterPosition{ FileString.find(EndDelimiter) };

	std::string LineBreak{ '#' };
	std::string LineBreakChar{ '\n' };
	if (FileString.find(LineBreak))
	{

		size_t Position = 0;
		while((Position = FileString.find(LineBreak)) != std::string::npos)
		{
				FILEDEBUG(std::cerr << "Found a linebreak!" << std::endl; );
			FileString.replace(Position, LineBreak.length(), LineBreakChar);
		}
	}
	return FileString.substr(StartOfSubstring, LastDelimiterPosition - StartOfSubstring);
}

void CGameData::ReadFileContent()
{
	std::vector<Location> tempLocationVector;

	if (_LocationFileNames.size() != 0)
	{
		for (size_t i{ 0 }; i < _LocationFileNames.size(); ++i)
		{
				FILEDEBUG(std::endl(std::cerr); );
			std::string ReadThis{ ConvertWString(_LocationFileNames[i]) };
			std::ifstream fLocationFile(ReadThis);
			int Lines{ DetermineAmountOfLines(ReadThis) };
			if (Lines == 0)
			{
				std::cerr << "<<There are no recorded lines in this Content file!" << std::endl;
			}
			else
			{
				Location tempLocation{};
				while (fLocationFile.is_open())
				{
					for (size_t j{ 0 }; j < Lines; ++j)
					{
						std::string String{ "" };
						std::getline(fLocationFile, String, '\n');
						if (String.at(0) == _InFileNamePrefix)
						{
							String = FormatString(String, GetStartContentDelimiter(), GetEndContentDelimiter());
								STRINGDEBUG(std::cerr << "@@Name is >>_" << String << "_<<" << std::endl; );
							tempLocation.ID = String;
						}
						else if (String.at(0) == _InFileChoicePrefix)
						{
							std::string SecondString{ "" };
							FormatStrings(String, SecondString);
							String = FormatString(String, GetStartContentDelimiter(), GetEndContentDelimiter());
							SecondString = FormatString(SecondString, GetStartContentDelimiter(), GetEndContentDelimiter());
								STRINGDEBUG(std::cerr << "@@Choice ID >>_" << String << "_<< has Choice Text >>_" << SecondString << "_<<" << std::endl; );
							tempLocation.Choices.push_back(LocationChoices(String, SecondString));
						}
						else if (String.at(0) == _InFileTextPrefix)
						{
							String = FormatString(String, GetStartContentDelimiter(), GetEndContentDelimiter());
								STRINGDEBUG(std::cerr << "@@Text is >>_" << String << "_<<" << std::endl; );
							tempLocation.Text = String;
						}
						else if (String.at(0) == _InFileCommentPrefix)
						{
							continue;
						}
						else
						{
							STRINGDEBUG(std::cerr << ">> " << String.at(0) << " is not a valid prefix!" << std::endl; );
						}
					};
					fLocationFile.close();
					tempLocationVector.push_back(tempLocation);
				}
			}
		}
	}
	else
	{
		std::cerr << "<<There are no recorded Location file names!" << std::endl;
	}
	//FILEDEBUG(std::cerr << "Size of _STestLocations is " << _STestLocations.size() << std::endl; );
	PushLocations(tempLocationVector);
		FILEDEBUG(PrintLocations(_SLocations); );
		FILEDEBUG(DebugLocations(); );
		FILEDEBUG(std::cerr << "Size of _SLocations is " << _SLocations.size() << std::endl; );
	//PrintLocations(GetTestLocations());
	//FILEDEBUG(std::cerr << "Size of _STestLocations is " << _STestLocations.size() << std::endl; );

	//skapar en rekursiv cykel
	//CGameData& Data = CGameData::AccessInstance();
	//for (auto& i : tempLocationVector)
	//{
	//	Data._STestLocations.push_back(i);
	//}
	//Data.PrintLocations(Data._STestLocations);
}

void CGameData::PushLocations(std::vector<Location>& Locations)
{
	for (auto& i : Locations)
	{
		//_STestLocations.push_back(i);
		_SLocations.push_back(i);
	}
	//_SLocations.push_back(Locations.at(0));														//TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//What we choose to do in the Location, exists in a Location /*Convenience Constructors, create more items with less lines of code*/
LocationChoices::LocationChoices(const std::string& NextID, const std::string& aText)
{
	LocationID = NextID;
	Text = aText;
}

//The Location, also has a vector for LocationChoices called Choices
Location::Location(const std::string& AnID, const std::string& aText)
{
	ID = AnID;
	Text = aText;
}

void CGameData::Setup()
{
	//CReader& Reader = CReader::AccessInstance();
	//CReader Reader;
	//Reader.ReadContentDirectory();
	//IsDirectoryEmpty();
	ReadContentDirectory();
}

CGameData::CGameData()
{
	//So CGameData returns here when we want to call private members of CGameData
	static bool _IsGameSetup{ false };

	if (!_IsGameSetup)
	{
		Setup();
		_IsGameSetup = true;

		//CreateLocations();
			DEBUG(PrintLocations(_SLocations); );
			DEBUG(DebugLocations(); );
	}
}

//CGameData& CGameData::AccessInstance()
//{
//	static CGameData Instance;
//	return Instance;
//}

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
	return false;
}
/*
void CGameData::CreateLocations()
{

	Location start(_STARTLOCATION, "Welcome to The Story, %%NAME%%! Please enjoy your stay!");
	start.Choices.push_back(LocationChoices("entrance", "Continue"));
	start.Choices.push_back(LocationChoices("exit", "Quit"));
	_SLocations.push_back(start);

	Location entrance("entrance", "You are leisurely walking through the forest when you spot the entrance to a cave nearby.");
	entrance.Choices.push_back(LocationChoices("room1", "Enter"));
	entrance.Choices.push_back(LocationChoices("exit", "Leave"));
	_SLocations.push_back(entrance);

	Location room1("room1", "You are in the first room of this story.");
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
*/
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
		CPlayer& Player = CPlayer::AccessInstance();
		std::regex nameRegex("%%NAME%%");
		std::string RegexLocation{ std::regex_replace(Location.Text, nameRegex, Player.GetPlayerName()) };
		std::cout << "\n" << RegexLocation;
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
		DEBUG(
			std::endl(std::cerr);
			for (size_t i{ 0 }; i < _SVisitedLocations.size(); ++i)
			{
				std::cerr << "Visited Locations: " << _SVisitedLocations[i].ID << "\n";
			} 
		);
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
			DEBUG(std::cerr << "<<We have stored Locations but current Location does not match a stored location" << std::endl; );
		return true;
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
		//else
		//{
		//	//std::cerr << "FindLocation could not find the chosen LocationID" << std::endl;		//triggers but gets blocked by cls
		//	//exit(EXIT_FAILURE);
		//}
	}
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
		exit(EXIT_FAILURE);
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



