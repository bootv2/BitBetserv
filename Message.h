#pragma once
class Message
{
private:
	std::string from, to, contents, ID;
public:
	Message();
	void genRandMessageID(int &ssalt);
	void setMessageContent(std::string content);
	std::string getFilteredMessageContent();
	std::string filterMessageContent(std::string content);
	~Message();
};

