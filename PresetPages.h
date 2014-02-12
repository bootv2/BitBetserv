#include <map>
#include "definitions.h"
#include <iostream>
#include <string>
#include "FileIO.cpp"
#include "page.cpp"

void setPresets(std::vector<Page>* _pMap, std::string* sessionID)
{
	FileIO htmlIO("foundation/", "index.html", true);
	std::vector<std::string> loadedPages;
	int u = 0;
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();

	_pMap->emplace_back(Page(loadedPages.at(u)));//0: index
	u++;
	/*_pMap->emplace_back(Page("<h1>Welcome to BitBet server Alpha 0.2</h1>"
		"<a href='/3'> Click here to log in!</a><br/>"
		"<a href='/8'> Click here to save all</a><br/>"
		"<a href='/1'> Click here create a new account!</a><br/>"));*/

	htmlIO.initHTML("Foundation/", "login.html");
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();

	_pMap->emplace_back(Page(loadedPages.at(u)));//1: Log In
	u++;

	htmlIO.initHTML("Foundation/", "createaccount.html");
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();

	_pMap->emplace_back(Page(loadedPages.at(u)));//2: Create Account
	u++;

	_pMap->emplace_back(Page("<h1>Please enter the desired roundName, startingPrice, growth rate, and running time below</h1>"//3: createRound
		"<form action='/7?session=^^'"
		"<table>"
		"Round name: <td><input name=roundName></td>"
		"Starting Price: <td><input name=startingPrice></td>"
		"Growth rate/buy in: <td><input name=growth></td>"
		"Running time in days: <td><input name=runningTime></td>"
		"</table>"
		"<input type=submit></form>"
		, sessionID));//TODO create a form that accepts round creation.

	_pMap->emplace_back(Page("<h1>Please enter your login credentials.</h1>" //4: Login POST
		"<form name='authentication' action='/4' method='post'>" //<form name="input" action="html_form_action.asp" method="post">
		"<table>"
		"<tr><td>Username</td><td><input name=Username></td></tr>"
		"<tr><td>Password</td><td><input type='password' name=Password></td></tr>"
		"</table>"
		"<input type=submit></form>"));

	htmlIO.initHTML("Foundation/css/", "foundation.css");

	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();//5: CSSfoundation

	_pMap->emplace_back(loadedPages.at(u));

	u++;

	htmlIO.initHTML("Foundation/js/vendor/", "modernizr.js");

	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}


	_pMap->emplace_back(loadedPages.at(u));//6: modernizr.js
	htmlIO.closeFile();

	u++;

	htmlIO.initHTML("Foundation/js/vendor/", "jquery.js");

	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}


	_pMap->emplace_back(loadedPages.at(u));//7: jquery.js
	htmlIO.closeFile();
	u++;

	htmlIO.initHTML("Foundation/js/", "foundation.min.js");
		
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();
	_pMap->emplace_back(loadedPages.at(u));//8: JSFoundationMin
	
	u++;

	htmlIO.initHTML("Foundation/js/foundation/", "foundation.js");

	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();
	_pMap->emplace_back(loadedPages.at(u));//9: JSFoundation

	htmlIO.initHTML("Foundation/", "badlogin.html");
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();

	_pMap->emplace_back(Page(loadedPages.at(u)));//10: loginparams: badlogin
	u++;

	htmlIO.initHTML("Foundation/", "alreadyexisting.html");
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();

	_pMap->emplace_back(Page(loadedPages.at(u)));//11: create account: account exists
	u++;

	htmlIO.initHTML("Foundation/", "accountcreated.html");
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();

	_pMap->emplace_back(Page(loadedPages.at(u)));//12: Create Account: account created!
	u++;

	htmlIO.initHTML("Foundation/", "roundlist.html");
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();

	_pMap->emplace_back(Page(loadedPages.at(u)));//13: BitBet: list Rounds
	u++;

	htmlIO.initHTML("Foundation/", "roundentry.html");
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();

	_pMap->emplace_back(Page(loadedPages.at(u)));//14: BitBet: list Rounds: round entry
	u++;

	htmlIO.initHTML("Foundation/", "motdpage.htm");
	loadedPages.emplace_back("");
	for (int i = 0; i < htmlIO.amountOfLines(); i++)
	{
		loadedPages.at(u) += htmlIO.readLine(i);
	}

	htmlIO.closeFile();

	_pMap->emplace_back(Page(loadedPages.at(u)));//15: Login Success!: motdpage
	u++;




}