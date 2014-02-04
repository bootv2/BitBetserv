#pragma once
class BitBet
{
private:
	std::vector<Round> roundStorage;
	Round _tmp;
public:
	BitBet()
	{
		newRound("testRound", 0.02, 0.02, 1000);

	}

	void newRound(std::string name, double startingAmount, double growth, int sSalt)
	{
		_tmp.init(name, growth, startingAmount, sSalt);
		roundStorage.emplace_back(_tmp);
	}

	std::string listRounds(std::string SesID)
	{
		std::string ret = "";
		for (auto& Round : roundStorage)
		{
			ret += "<br><a href='/bitbet?session=" + SesID + "&gameid=" + Round.getRoundID() + "'>" + Round.getName() + "</a> \n";
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

