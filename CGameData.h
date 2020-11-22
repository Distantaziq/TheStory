#pragma once
#include<iostream>
#include<string>
#include<vector>


//Parent class, other classes inherit from this one
/*
struct LocationActions
{
	LocationActions(const std::string& anAction, const std::string actionText);

	std::string ActionID{ "" };
	std::string Text{ "" };

};
*/
struct LocationChoices
{
	LocationChoices(const std::string& NextID, const std::string& aText);

	std::string LocationID{ "" };
	std::string Text{ "" };
};

struct Location
{
	Location(const std::string& AnID, const std::string& aText);

	std::string ID{ "" };
	std::string Text { "" };
	std::vector<LocationChoices> Choices;
	//std::vector<LocationActions> Actions;
};

class CGameData
{
public:
	CGameData();

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

private:
	const std::string _STARTLOCATION{ "start" };
	const std::string _EXITLOCATION{ "exit" };
	Location _CurrentLocation{ "", "" };
	Location _NextLocation{ "", "" };

	void CreateLocations();
	std::vector<Location> _SLocations;
	std::vector<Location> _SVisitedLocations;

};

