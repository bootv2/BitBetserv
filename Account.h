#pragma once
class Account
{
private:
	string username;
	string password;
	int* id;
	double* wallet = 0;
	short* status = 0;//0 for standard user, 1 for standard moderator, 2 for extended moderator, 3 for full admin
public:
	Account(string usr, string passw, int ID)
	{
		username = usr;
		password = passw;
		id = new int(ID);
		wallet = new double(0);
		status = new short(0);
	}

	void init(string usr, string passw, int ID)
	{
		username = usr;
		password = passw;
		id = new int(ID);
		wallet = new double(0);
		status = new short(0);
	}

	string getPass()
	{
		return password;
	}

	void setStatus(short _st)
	{
		status = new short(_st);
	}

	short getStatus()
	{
		return *status;
	}

	string getUsername()
	{
		return username;
	}

	int getID()
	{
		return *id;
	}

	bool auth(string usr, string passw)
	{
		if (usr == username && passw == password)
			return true;
		else
			return false;
	}

	double getWallet()
	{
		return *wallet;
	}

	void takeMoney(double moneyToTake)
	{
		*wallet -= moneyToTake;
	}

	void giveMoney(double moneyToGive)
	{
		*wallet += moneyToGive;
	}
};

