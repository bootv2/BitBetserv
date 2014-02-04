#pragma once
class Place
{
public:
	Place()
	{

	}
	Place(int id, double _money)
	{
		playerID = id;
		money = _money;
	}
	int playerID;
	double money = 0;
};

