#pragma once
class Place
{
private:
	int *playerID;
	double *money;
	int *placenum;
public:
	Place()
	{
		placenum = new int(0);
		playerID = new int(0);
		money = new double(0);
	}
	Place(int id, double _money, int _pID)
	{
		placenum = new int(_pID);
		playerID = new int(id);
		money = new double(0);
	}
	~Place()
	{
	}
	double getMoney()
	{
		return *money;
		//return 0.05;
	}

	void giveMoney(double _give)
	{
		*money += _give;
	}

	int getPlayerID()
	{
		std::cout << "PlayerID for round saving: " << std::to_string(*playerID) << std::endl;
		return *playerID;
	}

	int getPlaceNum()
	{
		return *placenum;
	}
};

