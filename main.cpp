#include "webserver.h"
#include "Socket.h"
#include <vector>
#include "account.cpp"
#include <iostream>
#include <random>
#include <time.h>
#include "Session.cpp"
#include "md5.h"
#include "page.cpp"
#include "PresetPages.h"
#include "BitBet.cpp"
#include "FileIO.cpp"

std::vector<Account> authenticationStorage;

std::vector<Session> sessionStorage;

std::vector<Page> pageStor;

BitBet defaultBitBet;

std::vector<std::string> pagePaths;
unsigned int seedsalt = 0;

std::string tmpSesID;

//Session defaultSession;

std::string getAnswer(std::string _body, std::string _title, std::string _bgcolor)
{
	std::string ans;
	ans = "<html><head><title>";
	ans += _title;
	ans += "</title></head><body bgcolor='" + _bgcolor + "'>";
	ans += _body;
	ans += "</body></html>";
	return ans;
}

void Get(webserver::http_request* r) {
  Socket s = *(r->s_);

  srand(time(NULL) + seedsalt);
  seedsalt++;

  std::string title;
  std::string body;
  std::string bgcolor="#ffffff";
  std::string links;

  std::cout << "request made to 127.0.0.1" << r->path_ << std::endl;

  bool dynamicPage = false;

  if (r->path_ == "/")
  {
	  body = pageStor.at(0).getBody();

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  return;
  }

  else if (r->path_ == "/saveAuth")
  {
	  FileIO authIO;
	  authIO.initNoInput("authorisation.dbs");
	  for (auto& Account : authenticationStorage)
	  {
		  authIO.writeString(Account.getUsername() + ":" + std::to_string(Account.getWallet()) + ":" + std::to_string(Account.getStatus()) + ":" + 
			  std::to_string(Account.getID()) + ":" + Account.getPass() + "\n");

	  }
	  body = "Successfully saved all users to the authorisation file\n";
	  std::cout << body;
	  r->answer_ = getAnswer(body, title, bgcolor);
	  r->params_.clear();
	  return;
  }

  else if (r->path_ == "/login")
  {
	  body = pageStor.at(1).getBody();

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  return;
  }

  else if (r->path_ == "/createaccount")
  {
	  body = pageStor.at(2).getBody();

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  return;
  }

  else if (r->path_ == "/bitbet")
  {
	  int curAccID;
	  body = "";
	  //account association.
	  for (auto& Session : sessionStorage)
	  {
		  if (r->params_["session"] == Session.getSessionID())
		  {
			  for (auto& Account : authenticationStorage)
			  {
				  if (Session.getAccountID() == Account.getID())
				  {
					  body = "Successfully gotten account " + Account.getUsername() + " associated with the current session<br>";
					  tmpSesID = Session.getSessionID();
					  curAccID = Account.getID();
				  }
			  }
		  }
	  }
	  //TODO: what happens if user isnt logged in.
	  if (body == "")
	  {
		  body = "<br><a href='/login'>You need to be logged in to play, click here to log in!</a> ";
		  r->answer_ = getAnswer(body, title, bgcolor);
		  return;
	  }

	  //TODO: what happens if the user opens a game.
	  for (auto& Round : defaultBitBet.getRounds())
	  {
		  if (r->params_["gameid"] == Round.getRoundID())
		  {
			  if (r->params_["buyin"] == "true")
			  {
				  for (auto& Account : authenticationStorage)
				  {
					  if (Account.getID() == curAccID)
					  {
						  if (Account.getWallet() > Round.getBuyInPrice())
						  {
							  Round.buyIn(Account);
						  }
					  }
				  }
			  }
			  body += Round.getRoundPage(tmpSesID);
			  body += "Listing other available rounds below. <br>";
		  }
	  }

	  //listing available rounds.
	  body += defaultBitBet.listRounds(tmpSesID);
	  r->answer_ = getAnswer(body, title, bgcolor);
	  return;
  }

  else if (r->path_ == "/give100creds")
  {
	  body = "";
	  //account association.
	  for (auto& Session : sessionStorage)
	  {
		  if (r->params_["session"] == Session.getSessionID())
		  {
			  for (auto& Account : authenticationStorage)
			  {
				  if (Session.getAccountID() == Account.getID())
				  {
					  body = "Successfully gotten account " + Account.getUsername() + " associated with the current session<br>";
					  tmpSesID = Session.getSessionID();
					  Account.giveMoney(100);
				  }
			  }
		  }
	  }
  }

  else if (r->path_ == "/roundmoneymade")
  {
	  body = "";
	  //account association.
	  for (auto& Session : sessionStorage)
	  {
		  if (r->params_["session"] == Session.getSessionID())
		  {
			  for (auto& Account : authenticationStorage)
			  {
				  if (Session.getAccountID() == Account.getID())
				  {
					  body = "Successfully gotten account " + Account.getUsername() + " associated with the current session<br>";
					  tmpSesID = Session.getSessionID();

					  for (auto& Round : defaultBitBet.getRounds())
					  {
						  if (r->params_["gameid"] == Round.getRoundID())
						  {
							  body += "you have made " + std::to_string(Round.getPlace(Session.getAccountID()).money) + " total in " + Round.getName() + "<br>";
						  }
					  }
				  }
			  }
		  }
	  }
  }

  else if (r->path_ == "/buyin")
  {
	  body = "";
	  //account association.
	  for (auto& Session : sessionStorage)
	  {
		  if (r->params_["session"] == Session.getSessionID())
		  {
			  for (auto& Account : authenticationStorage)
			  {
				  if (Session.getAccountID() == Account.getID())
				  {
					  body = "Successfully gotten account " + Account.getUsername() + " associated with the current session<br>";
					  tmpSesID = Session.getSessionID();
					  for (auto& Round : defaultBitBet.getRounds())
					  {
						  if (r->params_["gameid"] == Round.getRoundID())
						  {
							  if (Account.getWallet() > Round.getBuyInPrice())
							  {
								  std::cout << Account.getUsername() << " successfully baught into " << Round.getName() << std::endl;
								  Round.buyIn(Account);
								  body += "Successfully baught in! you can track the money you made in this game <a href = '/roundmoneymade?session=" + tmpSesID + "&gameid=" + r->params_["gameid"] + "'>here< / a> ";
							  }
							  else
							  {
								  std::cout << "Wallet contains " << Account.getWallet() << " credits, buyIN costs " << Round.getBuyInPrice() << std::endl;
								  body += "Insufficient funds! Click <br><a href = '/give100creds?session=" + tmpSesID + "&gameid=" + r->params_["gameid"] + "'>here< / a> to return to the round's page.<br>";
							  }
						  }
					  }
				  }
			  }
		  }
	  }
	  //TODO: what happens if user isnt logged in.
	  if (body == "")
	  {
		  body = "<br><a href='/login'>You need to be logged in to play, click here to log in!</a> ";
		  r->answer_ = getAnswer(body, title, bgcolor);
	  }

  }

	 else if (r->path_ == "/params.login")
	  {
		  bool correctAuthentication = false;
			  for (auto& Account : authenticationStorage)
			  {
				  if (Account.auth(r->params_["Username"], r->params_["Password"]))
				  {
					  correctAuthentication = true;
					  cout << "auth correct<br>";
					  sessionStorage.emplace_back(Session(seedsalt, Account.getID()));
					  tmpSesID = sessionStorage.at(sessionStorage.size() - 1).getSessionID();
					  break;
				  }
			  }

		  if (!correctAuthentication)
		  {
			  body = "<p><a href='/login'>Incorrect Authentication... Click here to retry.</a>";
		  }
		  else
		  {
			  body = "<h1>User " + r->params_["Username"] + " authenticated successfully!</h1><br>";
			  body += "<br><a href='/bitbet?session=" + tmpSesID + "'>Click here to enter BitBet</a> <br>";
		  }
	  }

	  else if ("/params.entergame.accountcreate")
	  {
		  bool accountExisting = false;
		  //bool authenticated = false;
		  for (auto& Account : authenticationStorage)
		  {
			  if (Account.getUsername() == r->params_["Username"])
			  {
				  body = "<h1> Account " + r->params_["Username"] + " already exists, please try another username. </h1>";
				  body += "<form action='/entergame/accountcreate/param'>"
					  "<table>"
					  "<tr><td>Field 1</td><td><input name=Username></td></tr>"
					  "<tr><td>Field 2</td><td><input name=Password></td></tr>"
					  "</table>"
					  "<input type=submit></form>";
				  accountExisting = true;
				  //authenticated = true;
				  break;
			  }
		  }
		  if (!accountExisting)
		  {
			  body = "<br><a href='/login'>Click here to return to the login screen</a> ";
			  authenticationStorage.emplace_back(Account(r->params_["Username"], r->params_["Password"], authenticationStorage.size() + 1));
		  }
		  else
		  {
			  cout << "account exists";
		  }
	  }
	  else {
		  r->status_ = "404 Not Found";
		  title = "Wrong URL";
		  body = "<h11>Wrong URL</h11>";
		  body += "Path is : &gt;" + r->path_ + "&lt;";
	  }

	  r->answer_ = "<html><head><title>";
	  r->answer_ += title;
	  r->answer_ += "</title></head><body bgcolor='" + bgcolor + "'>";
	  r->answer_ += body;
	  r->answer_ += "</body></html>";

	  r->params_.clear();
  
}

int main() {
	setPresets(&pageStor);
	Account admin("bootv2", "1234", 0);
	admin.giveMoney(10000);
	admin.setStatus(3);
	authenticationStorage.emplace_back(admin);
	std::cout << "attempting to start the server on port 8081" << std::endl;
	webserver(8081, Get);

}
