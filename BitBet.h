#pragma once
class BitBet
{
private:
	std::vector<Round> roundStorage;
	Round* _tmp;
	std::vector<Account>* pAccStor;
public:
	BitBet(std::vector<Account>* _p)
	{
		pAccStor = _p;
		newRound("TestRound", 0.05f, 0.05f, 1000, 5);
		newRound("secondTest", 1.05f, 1.05f, 2000, 5);

	}

	void newRound(std::string name, double startingAmount, double growth, int sSalt, int _days)
	{
		_tmp = new Round(name, startingAmount, growth, sSalt, pAccStor);
		_tmp->setMaxRunningTime(_days);
		roundStorage.emplace_back(*_tmp);
		delete _tmp;
	}

	std::vector<std::string> getListedRounds()
	{
		std::vector<std::string> ret;
		for (auto& Round : roundStorage)
		{
			ret.emplace_back(Round.getName());
		}
		return ret;
	}

	int amountOfRounds()
	{
		return roundStorage.size();
	}

	Round getRoundFromId(int id)
	{
		return roundStorage.at(id);
	}

	std::string listRounds(std::string SesID)
	{
		std::string ret = "";
		for (auto& Round : roundStorage)
		{
			ret += "<br><a href='/5?session=" + SesID + "&gameid=" + Round.getRoundID() + "'>" + Round.getName() + "</a> \n";
		}

		return ret;
	}

	/*Round getRound(std::string name)
	{
		return roundStorage[name];
	}*/

	std::vector<Round> getRounds()
	{
		return roundStorage;
	}
};

