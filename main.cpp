#include "webserver.h"
#include "Socket.h"
#include "definitions.h"
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
#include "LoadingClass.h"
#include "HTMLVariables.h"


std::vector<Account> authenticationStorage;

std::vector<Session> sessionStorage;

std::vector<Page> pageStor;

BitBet defaultBitBet(&authenticationStorage);

std::vector<std::string> pagePaths;
unsigned int seedsalt = 0;

std::string tmpSesID;

LoadingClass load(&authenticationStorage, &defaultBitBet);

std::string currentSessionID;

std::string MOTD;

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

  const int home(0), createAccount(1), createAccountParams(2), login(3), loginParams(4), bitbet(5), staticNewRound(6), newRoundParams(7),
	  saveAll(8), give100Credits(9), roundMoneyMade(10), buyin(11);

  srand(time(NULL) + seedsalt);
  seedsalt++;

  std::string title;
  std::string body;
  std::string bgcolor="#ffffff";
  std::string links;
  currentSessionID = r->params_["session"];

  HTMLVariables HTMLvars;
  HTMLVariables entryVars;

  bool accountExisting = false;
  bool correctAuthentication = false;

  //Make switch for page switching functions
  std::cout << "request made to server:" << r->path_ << std::endl;

  if (r->path_ == "/")
	  r->path_ = "/0";

  std::cout << "compensated for atoi reading: " << r->path_ << std::endl;

  switch (atoi(r->path_.substr(1, r->path_.size()).c_str()))
  {
  case home:
	  body = pageStor.at(0).getBody();

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  break;
  case createAccount:
	  body = pageStor.at(2).getBody();

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  break;

  case login:
	  body = pageStor.at(1).getBody();

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  break;

  case loginParams:
	  cout << "entered loginparams..\n" << r->params_["-Username"] << endl;
	  for (auto& Account : authenticationStorage)
	  {
		  if (Account.auth(r->params_["-Username"], r->params_["-Password"]))
		  {
			  correctAuthentication = true;
			  cout << "auth correct\n";
			  sessionStorage.emplace_back(Session(seedsalt, Account.getID()));
			  tmpSesID = sessionStorage.at(sessionStorage.size() - 1).getSessionID();
			  break;
		  }
	  }

	  if (!correctAuthentication)
	  {
		  body = pageStor.at(10).getBody();
	  }
	  else
	  {
		  HTMLvars.init(pageStor.at(15).getBody());
		  HTMLvars.variables["sess"] = tmpSesID;
		  body = HTMLvars.writeVars();
		  HTMLvars.reset();
	  }

	  r->answer_ = body;

	  r->params_.clear();
	  break;

  case bitbet:
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
		  body = "<br><a href='/" + std::to_string(login) + "'>You need to be logged in to play, click here to log in!</a> ";
		  r->answer_ = getAnswer(body, title, bgcolor);
		  break;
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
	  HTMLvars.init(pageStor.at(13).getBody());
	  HTMLvars.variables["rent"] = "";
	  for (auto& Account : authenticationStorage)
	  {
		  if (curAccID == Account.getID())
		  {
			  HTMLvars.variables["user"] = Account.getUsername();
			  HTMLvars.variables["usr1"] = Account.getUsername();
		  }
	  } 
	  for (auto& Round : defaultBitBet.getRounds())
	  {
		  entryVars.init(pageStor.at(14).getBody());
		  entryVars.variables["rnam"] = Round.getName();
		  entryVars.variables["ratt"] = Round.getRoundAttributes();
		  HTMLvars.variables["rent"] += entryVars.writeVars();
		  std::cout << entryVars.writeVars();
		  entryVars.reset();
	  }
	  body = HTMLvars.writeVars();
	  HTMLvars.reset();
	  r->answer_ = getAnswer(body, title, bgcolor);
	  r->params_.clear();
	  break;

  case createAccountParams:
	  for (auto& Account : authenticationStorage)
	  {
		  if (Account.getUsername() == r->params_["-Username"])
		  {
			  HTMLvars.init(pageStor.at(11).getBody());
			  HTMLvars.variables["user"] = r->params_["-Username"];
			  body = HTMLvars.writeVars();
			  HTMLvars.reset();
			  accountExisting = true;
			  break;
		  }
	  }
	  if (!accountExisting)
	  {
		  HTMLvars.init(pageStor.at(12).getBody());
		  HTMLvars.variables["user"] = r->params_["-Username"];
		  body = HTMLvars.writeVars();
		  HTMLvars.reset();
		  authenticationStorage.emplace_back(Account(r->params_["-Username"], r->params_["-Password"], authenticationStorage.size() + 1));
	  }
	  else
	  {
		  cout << "account exists\n";
	  }
	  r->answer_ = getAnswer(body, title, bgcolor);
	  r->params_.clear();
  break;

  case staticNewRound:
	  body = pageStor.at(3).getBody();

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  break;

  case newRoundParams:
	  defaultBitBet.newRound(r->params_["roundName"], atof(r->params_["startingPrice"].c_str()), atof(r->params_["growth"].c_str()), 
		  seedsalt, atoi(r->params_["runningTime"].c_str()));

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  break;

  case saveAll:
	  load.saveBitBet();
	  load.saveAuthorisations();
	  body = "Successfully saved all users to the authorisation file\n";
	  std::cout << body;

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  break;

  case give100Credits: // Create payment screen for adding credits.
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
					  body = "Successfully gotten account " + Account.getUsername() + " associated with the current session<br>"
						  "<a href='/5?gameid=" + r->params_["gameid"] + "&session=" + r->params_["session"] + "'>Click here to return to the previous round page.</a>";
					  tmpSesID = Session.getSessionID();
					  Account.giveMoney(100);
				  }
			  }
		  }
	  }

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  break;

  case roundMoneyMade:
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
							  body += "you have made " + std::to_string(Round.getPlace(Session.getAccountID()).getMoney()) + " total in " + Round.getName() + "<br>";
						  }
					  }
				  }
			  }
		  }
	  }

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  break;

  case buyin:
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
								  body += "Successfully baught in! you can track the money you made in this game <a href = '/" + std::to_string(roundMoneyMade) + "?session=" + tmpSesID + "&gameid=" + r->params_["gameid"] + "'>here< / a> ";
							  }
							  else
							  {
								  std::cout << "Wallet contains " << Account.getWallet() << " credits, buyIN costs " << Round.getBuyInPrice() << std::endl;
								  body += "Insufficient funds! Click <br><a href = '/" + std::to_string(give100Credits) + "?session=" + tmpSesID + "&gameid=" + r->params_["gameid"] + "'>here< / a> to get 100 credits.<br>";
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
		  body = "<br><a href='/" + std::to_string(login) + "'>You need to be logged in to play, click here to log in!</a> ";
		  r->answer_ = getAnswer(body, title, bgcolor);
	  }

	  r->answer_ = getAnswer(body, title, bgcolor);

	  r->params_.clear();
	  break;

  case 401:
	  r->answer_ = getAnswer(pageStor.at(4).getBody(), title, bgcolor);

	  r->params_.clear();
	  break;

  case 1000: //CSS page series(1000-1999)
	  r->answer_ = pageStor.at(5).getBody();//get CSS foundation sheet

	  r->params_.clear();
	  break;

  case 2000: //JS page series(2000-2999)
	  r->answer_ = pageStor.at(6).getBody();//get JS mordernizr

	  r->params_.clear();
	  break;

  case 2001:
	  r->answer_ = pageStor.at(7).getBody();//get JS/vendor/jquery

	  r->params_.clear();
	  break;

  case 2002:
	  r->answer_ = pageStor.at(8).getBody();//get JS foundation.min

	  r->params_.clear();
	  break;

  case 2003:
	  r->answer_ = pageStor.at(9).getBody();//get JS foundation

	  r->params_.clear();
	  break;


  default:
	  //404 page
	  body = "404, page not found!";
	  title = body;
	  r->answer_ = getAnswer(body, title, bgcolor);
	  r->params_.clear();
	  break;
  }

  
  
}

int main() {
	load.loadAuthorisations();
	setPresets(&pageStor, &currentSessionID);
	std::cout << "attempting to start the server on port 8081" << std::endl;
	webserver(80, Get);

}
