#pragma once
class Session
{
private:
	std::string sessionID;
	int sessionAccountID;
	int uniqueSessionAuthID;
public:
	Session::Session(int sSalt, int accID)
	{
		sessionAccountID = accID;
		genRandSessionString(sSalt);
	}
	Session()
	{

	}

	std::string getSessionID()
	{
		return sessionID;
	}

	int getAccountID()
	{
		return sessionAccountID;
	}

	void genRandSessionString(unsigned int seedSalt)
	{
		std::string result = "";
		std::string availableChars = "abcdefghijklmnopqrstuvwxyz1234567890-_!";
		for (int i = 0; i < 25; i++)
		{
			srand(time(NULL) + seedSalt + i);
			result += availableChars.at(rand() % (availableChars.size() - 1));
			seedSalt++;
		}
		std::cout << result << std::endl;
		sessionID = result;
	}

	void init(int sesID, int accID, int unSesID)
	{
		sessionID = sesID;
		sessionAccountID = accID;
		uniqueSessionAuthID = unSesID;
	}
};

