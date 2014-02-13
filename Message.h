#pragma once
class Message
{
private:
	std::string from, to, contents, ID;
public:
	Message();
	void genRandMessageID(int &ssalt);
	~Message();
};

