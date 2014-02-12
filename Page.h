#pragma once
class Page
{
private:
	std::string body;
	Session currentSession;
	std::vector<std::string>* params;
	std::string* sessionID;
public:
	Page(int sesID, int accID, int unSesID, std::vector<std::string>* _params)
	{
		currentSession.init(sesID, accID, unSesID);
		params = _params;
	}

	Page(std::string _body, std::string* _sessionID)
	{
		body = _body;
		sessionID = _sessionID;
	}

	Page(std::string _body)
	{
		body = _body;
	}

	Page()
	{

	}

	void setSession()
	{
		std::vector<std::string> tmp;
		tmp = split(body, '^');
		for (int i = 0; i < tmp.size(); i++)
		{
			if (tmp.at(i).compare("") == 0)
				tmp.at(i) = *sessionID;
		}
		std::cout << *sessionID << std::endl;
		body.clear();
		for (int i = 0; i < tmp.size(); i++)
		{
			body += tmp.at(i);
		}
	}

	std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems)
	{
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	void setBody(std::string b)
	{
		body = b;
	}

	std::string getBody()
	{
		return body;
	}

};

