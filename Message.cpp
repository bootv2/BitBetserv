#include <string>
#include <random>
#include <time.h>

#include "Message.h"


Message::Message()
{
}

void Message::genRandMessageID(int &ssalt)
{
		std::string result = "";
		std::string availableChars = "abcdefghijklmnopqrstuvwxyz1234567890-_!";
		for (int i = 0; i < 25; i++)
		{
			srand(time(NULL) + ssalt + i);
			result += availableChars.at(rand() % (availableChars.size() - 1));
			ssalt++;
		}
		ID = result;
	
}

Message::~Message()
{
}
