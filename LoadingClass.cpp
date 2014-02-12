#include "Account.cpp"
#include <vector>
#include <iostream>
#include "FileIO.cpp"
#include <sstream>
#include "BitBet.cpp"
#include "Round.cpp"
#include "LoadingClass.h"


LoadingClass::LoadingClass(std::vector<Account>* _pAccStor, BitBet* _bb)
{
	bb = _bb;
	pAuthStorage = _pAccStor;
}

void LoadingClass::loadAuthorisations()
{
	loadingIO.init(authFileName);
	Account* tmp;
	bool validLine = false;
	string breakstring = ":";
	string str;
	int iterator = 0;
	for (int i = 0; i < loadingIO.amountOfLines(); i++) lines.emplace_back(loadingIO.readLine(i));
	std::vector<std::string> lineTokens;

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines.at(i) == "") continue;
		lineTokens = split(lines.at(i), ':');
		tmp = new Account(lineTokens.at(0), lineTokens.at(4), atoi(lineTokens.at(3).c_str()));
		tmp->giveMoney(atof(lineTokens.at(1).c_str()));
		tmp->setStatus(atoi(lineTokens.at(1).c_str()));
		std::cout << lineTokens.at(0) << " Loaded! password read as: " << lineTokens.at(4) << std::endl;
		pAuthStorage->emplace_back(*tmp);
		delete tmp;
		lineTokens.clear();
	}
	loadingIO.closeFile();
	saveAuthorisations();
}

void LoadingClass::saveAuthorisations()
{
	loadingIO.init(authFileName);
	for (auto& Account : *pAuthStorage)
	{
		loadingIO.writeString("&" + Account.getUsername() + ":" + std::to_string(Account.getWallet()) + ":" + std::to_string(Account.getStatus()) + ":" +
			std::to_string(Account.getID()) + ":" + Account.getPass());

	}
	loadingIO.closeFile();
}

void LoadingClass::saveBitBet()
{
	loadingIO.init(bbFileName);
	Round tmp;
	std::vector<std::string> roundNames = bb->getListedRounds();
	std::cout << "Found " << bb->amountOfRounds() << " rounds to save...&";
	for (int i = 0; i < bb->amountOfRounds(); i++)
	{
		loadingIO.writeString("round:" + roundNames.at(i) + "&");
		tmp = bb->getRoundFromId(i);
		loadingIO.writeString("places&");
		for (int x = 0; x < tmp.getAmountOfPlaces(); x++)
		{
			loadingIO.writeString(std::to_string(tmp.getPlace(x).getPlaceNum()) + ":" + std::to_string(tmp.getPlace(x).getPlayerID()) + ":" + std::to_string(tmp.getPlace(x).getMoney()));
			if (x + 1 > tmp.getAmountOfPlaces())
			{
				loadingIO.writeString(";");
			}
			loadingIO.writeString("&");
		}
		loadingIO.writeString(tmp.getRoundID() + ":" + std::to_string(tmp.getBuyInPrice()) + ":");
		for (int y = 0; y < 4; y++)
		{
			loadingIO.writeString(std::to_string(tmp.getTimeVector().at(y)) + ":");
		}
		loadingIO.writeString("&");
	}
	loadingIO.closeFile();
}

LoadingClass::~LoadingClass()
{
}

std::vector<std::string> &LoadingClass::split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> LoadingClass::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}