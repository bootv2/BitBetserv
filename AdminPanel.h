#pragma once
class AdminPanel
{
	std::vector<Account>* pAccStor;
	std::string accHTML;
	HTMLVariables HTMLvars;
public:
	AdminPanel();
	void init(std::vector<Account>* _pAcc, std::string acchtml);
	std::string listAccounts(std::string ses);
	~AdminPanel();
};

