#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<regex>

#include<fstream>
#include<tchar.h>
#include<wchar.h>
#include<windows.h>




//class CGameData;
//
//class CReader
//{
//public:
//	//CReader();
//	//static CReader& AccessInstance();
//
//	friend class CGameData;
//
//	bool IsFolderEmpty(int Items) { return Items <= 0; }
//
//
//
//
//
//
//
//
//private:
//
//
//};


struct LocationChoices
{
	LocationChoices(const std::string& NextID, const std::string& aText);

	std::string LocationID{ "" };
	std::string Text{ "" };
};

struct Location
{
	Location() = default;
	Location(const std::string& AnID, const std::string& aText);

	std::string ID{ "" };
	std::string Text{ "" };
	std::vector<LocationChoices> Choices;
};

class CPlayer;

//Parent class, other classes inherit from this one
class CGameData
{
public:
	//friend class CReader;
	CGameData();

	void UpdateLocationAmount(int NewInt) { _LocationAmount = NewInt; }
	LPCWSTR AccessContentFolder() const { return CONTENTFOLDER; }



	//static CGameData& AccessInstance();

	void DebugLocations();
	void PrintLocation(const Location& Location) const;
	void PrintLocations(const std::vector<Location>& LocationStorage) const;
	bool ChoicesLocationExistsWithID(const std::string& NextID);

	void PrintChoices(const Location& Location);
	void PrintTextAndChoices(const Location& Location);
	int CountChoices(const Location& Location) const;

	void AddToVisitedLocations(const Location& Location);
	bool IsLocationNew(const Location& Location) const;


	Location FindLocationFromChoice(const std::string& Locator) const;
	Location GetLocation(const std::string& Locator) const;
	Location GetNextLocation() const { return _NextLocation; }
	Location GetCurrentLocation() const { return _CurrentLocation; }

	void UpdateCurrentLocation(const Location& Location) { _CurrentLocation = Location; }
	void SetNextLocation(const Location& Location) { _NextLocation = Location; }
	bool IsExit(const Location& Location) const;
	bool IsStart(const Location& Location) const;

	const std::string GetStartLocation() const { return _STARTLOCATION; }

	std::vector<Location> GetTestLocations() const { return _STestLocations; }

private:
	void ReadContentDirectory();
	bool IsDirectoryEmpty() const { return _LocationAmount > 0; }
	void DebugPrintStoredFileNames();
	int DetermineAmountOfLines(const std::string& FileName);
	std::string ConvertWString(const std::wstring& ConvertThis);
	void FormatStrings(std::string& FirstString, std::string& SecondString);
	std::string FormatString(std::string& FileString, const std::string& StartDelimiter, const std::string& EndDelimiter);
	void ReadFileContent();
	void PushLocations(std::vector<Location>& Locations);

	const std::string GetStartContentDelimiter() const { return _StartContentDelimiter; }
	const std::string GetEndContentDelimiter() const { return _EndContentDelimiter; }
	const std::string GetStringDelimiter() const { return _StringDelimiter; }



	LPCWSTR CONTENTFOLDER{ L"Content/*.txt" };
	const std::string _ContentDirPrefix{ "Content/" };
	int _LocationAmount{ 0 };
	const wchar_t _FileLocationPrefix{ L'l' };				//prefix to identify l_Location files
	const char _InFileNamePrefix{ L'N' };					//Location ID in the file
	const char _InFileChoicePrefix{ L'C' };					//Location Choices in the file
	const char _InFileTextPrefix{ L'T' };					//Location Text in the file
	const char _InFileCommentPrefix{ L'#' };				//Location Text in the file
	const std::string _StartContentDelimiter{ "[" };
	const std::string _EndContentDelimiter{ "]" };
	const std::string _StringDelimiter{ "," };
	std::vector<std::wstring> _LocationFileNames{};



	const std::string _STARTLOCATION{ "start" };
	const std::string _EXITLOCATION{ "exit" };
	Location _CurrentLocation{};
	Location _NextLocation{};

	void Setup();
	void CreateLocations();
	std::vector<Location> _SLocations;
	std::vector<Location> _SVisitedLocations;
	std::vector<Location> _STestLocations;

};

