#pragma once
class Place
{
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
		//return *money;
		return 0.05;
	}

	void giveMoney(double _give)
	{
		*money += _give;
	}

	int getPlayerID()
	{
		return *playerID;
	}

	int getPlaceNum()
	{
		return *placenum;
	}

	int *playerID;
	double *money;
	int *placenum;
};

