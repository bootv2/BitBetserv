#include <vector>
#include <map>
#include <string>

#include "Account.cpp"
#include "HTMLVariables.h"
#include "AdminPanel.h"


AdminPanel::AdminPanel()
{
}

void AdminPanel::init(std::vector<Account>* _pAcc, std::string acchtml)
{
	accHTML = acchtml;
	pAccStor = _pAcc;
}

std::string AdminPanel::listAccounts(std::string ses)
{
	std::string ret;
	for (int i = 0; i < pAccStor->size(); i++)
	{
		HTMLvars.init(accHTML);
		HTMLvars.variables["acnm"] = pAccStor->at(i).getUsername();
		HTMLvars.variables["usid"] = pAccStor->at(i).getUsername();
		HTMLvars.variables["sess"] = ses;
		if (pAccStor->at(i).getStatus() > 0)
			HTMLvars.variables["acst"] = "admin";
		else
			HTMLvars.variables["acst"] = "user";
		if (pAccStor->at(i).isBanned())
		{
			HTMLvars.variables["band"] = "<font color='red'>Banned!</font>";
			HTMLvars.variables["uban"] = "Unban!";
		}
		else
		{
			HTMLvars.variables["band"] = "Not banned!";
			HTMLvars.variables["uban"] = "Ban!";
		}
		ret += HTMLvars.writeVars();
		HTMLvars.reset();
	}
	return ret;
}


AdminPanel::~AdminPanel()
{
}
