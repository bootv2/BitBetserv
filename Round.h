#pragma once
class Round
{
private:
	std::vector<Place> placeMap;
	double buyInPrice = 0;
	double growth = 0;

	bool running = true;

	unsigned int creationTime, runningTime, maxTime, days;

	std::string roundName;
	std::string roundID;

	std::vector<Account>* pAccStor;

	void payPlayers()
	{
		for (int i = 0; i < placeMap.size() - 1; i++)
			placeMap.at(i).money += growth/2;
	}
public:
	Round(std::string name, double startPrice, double _growth, int sSalt, std::vector<Account>* _pAccStor)
	{
		pAccStor = _pAccStor;
		creationTime = time(NULL);
		runningTime = 0;
		roundName = name;
		buyInPrice = startPrice;
		growth = _growth;
		genRoundID(sSalt);
	}

	void setMaxRunningTime(int _days)
	{ 
		days = _days;
		maxTime = time(NULL) + (days * 24 * 60 * 60);//converting days to minutes.
	}

	Place getPlace(int accID)
	{
		for (auto& Place : placeMap)
		{
			if (accID == Place.playerID)
			{
				return Place;
			}
		}
	}

	Round()
	{}

	void checkRunning()
	{
		
		if (runningTime > days * 24 * 60 * 60)
		{
			running = false;//give all players the money they made in this round
			for (auto& Place : placeMap)
			{
				for (int i = 0; i < pAccStor->size(); i++)
				{
					if (Place.playerID == pAccStor->at(i).getID())
					{
						pAccStor->at(i).giveMoney(Place.money);
					}
				}
			}
		}
	}

	std::string getName()
	{
		return roundName;
	}

	std::string getRoundPage(std::string sesID)
	{
		std::string ret = "";
		ret += "Welcome to BitBet round " + roundName + ".\n";
		ret += "BuyIN now costs: " + std::to_string(buyInPrice) + " credits.\n";
		ret += "The current growth is: " + std::to_string(growth) + " credits per BuyIN! \n";
		ret += "<br><a href='/buyin?session=" + sesID + "&gameid=" + roundID + "'>Click here to buy in!</a> \n";
		return ret;
	}

	double getBuyInPrice()
	{
		return buyInPrice;
	}

	void genRoundID(unsigned int sSalt)
	{
		std::string result = "";
		std::string availableChars = "abcdefghijklmnopqrstuvwxyz1234567890-_!";
		for (int i = 0; i < 5; i++)
		{
			srand(time(NULL) + sSalt + i);
			result += availableChars.at(rand() % (availableChars.size() - 1));
			sSalt++;
		}
		std::cout << result << std::endl;
		roundID = result;
	}

	std::string getRoundID()
	{
		return roundID;
	}

	void init(std::string name, double _growth, double startPrice, int sSalt)
	{
		roundName = name;
		buyInPrice = startPrice;
		growth = _growth;
		genRoundID(sSalt);
	}

	void buyIn(Account& _a)
	{
		runningTime = time(NULL) - creationTime;
		maxTime = time(NULL) + (days * 24 * 60 * 60);

		_a.takeMoney(buyInPrice);
		buyInPrice += growth;
		placeMap.emplace_back(Place(_a.getID(), 0));
		payPlayers();
	}
};

