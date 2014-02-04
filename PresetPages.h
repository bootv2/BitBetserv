#include <map>
#include <string>
#include "page.cpp"

void setPresets(std::vector<Page>* _pMap)
{
	_pMap->emplace_back(Page("<h11>Web Server Example</h11>"
		"I wonder what you're going to click"
		"<p><a href='/login'>Log in to the BitBet game.</a> "
		"<p><a href='/createaccount'>Create a new account for the BitBet game.</a>"));

	_pMap->emplace_back(Page("<h1>Please enter your login credentials.</h1>"
		"<form action='/params.login'>"
		"<table>"
		"<tr><td>Field 1</td><td><input name=Username></td></tr>"
		"<tr><td>Field 2</td><td><input name=Password></td></tr>"
		"</table>"
		"<input type=submit></form>"));

	_pMap->emplace_back(Page("<h1>Please enter your desired username and password here< / h1>"
		"<form action='/entergame/accountcreate/param'>"
		"<table>"
		"<tr><td>Field 1</td><td><input name=Username></td></tr>"
		"<tr><td>Field 2</td><td><input name=Password></td></tr>"
		"</table>"
		"<input type=submit></form>"));

}