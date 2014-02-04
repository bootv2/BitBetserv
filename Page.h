#pragma once
class Page
{
private:
	std::string body;
	Session currentSession;
	std::vector<std::string>* params;
public:
	Page(int sesID, int accID, int unSesID, std::vector<std::string>* _params)
	{
		currentSession.init(sesID, accID, unSesID);
		params = _params;
	}

	Page(std::string _body)
	{
		body = _body;
	}

	Page()
	{

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

