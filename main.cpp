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
#include "AdminPanel.h"



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
	  saveAll(8), give100Credits(9), roundMoneyMade(10), buyin(11), adminPanel(12), userPanel(13);

  srand(time(NULL) + seedsalt);
  seedsalt++;

  std::string title;
  std::string curAcc;
  std::string body;
  std::string bgcolor="#ffffff";
  std::string links;
  currentSessionID = r->params_["session"];

  HTMLVariables HTMLvars;
  HTMLVariables entryVars;

  AdminPanel adminPan;
  adminPan.init(&authenticationStorage, pageStor.at(20).getBody());

  bool accountExisting = false;
  bool correctAuthentication = false;
  bool admin = false;
  bool banned = false;

  //Make switch for page switching functions
  std::cout << "request made to server:" << r->path_ << std::endl;

  if (r->path_ == "/")
	  r->path_ = "/0";
  else if (r->path_ == "")
	  return;

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
			  if (Account.isBanned())
			  {
				  correctAuthentication = false;
				  banned = true;
			  }
			  sessionStorage.emplace_back(Session(seedsalt, Account.getID()));
			  tmpSesID = sessionStorage.at(sessionStorage.size() - 1).getSessionID();
			  break;
		  }
	  }

	  if (!correctAuthentication)
	  {
		  if (!banned)
			  body = pageStor.at(10).getBody();
		  else
		  {
			  banned = false;
			  body = "You are banned!";
		  }
	  }
	  else
	  {
		  HTMLvars.init(pageStor.at(15).getBody());
		  HTMLvars.variables["sess"] = tmpSesID;
		  MOTD = "Hello " + r->params_["-Username"] + ", it's good to see you again.<br><br>"
			  "Please play it safe, gamble, but dont spend any money you can't miss!";
		  HTMLvars.variables["motd"] = MOTD;
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
			  HTMLvars.variables["sess"] = r->params_["session"];
			  if (Account.getStatus() > 0)
				  admin = true;
		  }
	  } 
	  for (auto& Round : defaultBitBet.getRounds())
	  {
		  entryVars.init(pageStor.at(14).getBody());
		  entryVars.variables["rnam"] = Round.getName();
		  entryVars.variables["ratt"] = Round.getRoundAttributes();
		  entryVars.variables["sess"] = r->params_["session"];
		  entryVars.variables["roid"] = Round.getRoundID();
		  HTMLvars.variables["rent"] += entryVars.writeVars();
		  entryVars.reset();
	  }
	  if (admin)
	  {
		  admin = false;
		  HTMLvars.variables["atad"] = "<section class='section'>"
			  "<h5 class='title'><a href='/12?session=" + r->params_["session"] + "&page=0'>Admin Panel</a></h5>"
			  "</section>";
	  }
	  else
		  HTMLvars.variables["atad"] = "You need to be an administrator to access this panel!";
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
		  authenticationStorage.emplace_back(Account(r->params_["-Username"], r->params_["-Password"], authenticationStorage.size() + 1, false));
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

  case userPanel:
	  HTMLvars.init(pageStor.at(22).getBody());
	  HTMLvars.variables["ses0"] = r->params_["session"];
	  HTMLvars.variables["ses1"] = r->params_["session"];
	  HTMLvars.variables["ses2"] = r->params_["session"];
	  HTMLvars.variables["ses3"] = r->params_["session"];
	  HTMLvars.variables["ses4"] = r->params_["session"];
	  HTMLvars.variables["ses5"] = r->params_["session"];

	  if (r->params_["page"] == "0")
	  {
		  HTMLvars.variables["aloc"] = "Home";
		  HTMLvars.variables["adat"] = "Welcome to the user account panel!";
	  }

	  else if (r->params_["page"] == "1")
	  {
		  entryVars.init(pageStor.at(23).getBody());
		  entryVars.variables["sess"] = r->params_["session"];
		  HTMLvars.variables["aloc"] = "Security";
		  HTMLvars.variables["adat"] = entryVars.writeVars();
		  entryVars.reset();

	  }

	  else if (r->params_["page"] == "11")
	  {
		  HTMLvars.variables["aloc"] = "Security";
		  
		  for (auto& Session : sessionStorage)
		  {
			  if (Session.getSessionID() == r->params_["session"])
			  {
				  for (auto& Account : authenticationStorage)
				  {
					  if (Session.getAccountID() == Account.getID())
					  {
						  if (r->params_["oldPass"] == Account.getPass())
						  {
							  Account.setPass(r->params_["newPass"]);
							  HTMLvars.variables["adat"] = "Password has successfully been changed!";
						  }
						  else
							  HTMLvars.variables["adat"] = "The password you entered is invalid.";
					  }
				  }
			  }
		  }
	  }

	  r->answer_ = HTMLvars.writeVars();
	  HTMLvars.reset();
	  r->params_.clear();
	  break;

  case adminPanel:
	  for (auto& Session : sessionStorage)
	  {
		  if (Session.getSessionID() == r->params_["session"])
		  {
			  for (auto& Account : authenticationStorage)
			  {
				  if (Session.getAccountID() == Account.getID())
				  {
					  curAcc = Account.getUsername();
					  if (Account.getStatus() == 1)//admin status
						  admin = true;
					  else
						  admin = false;
				  }
			  }
		  }
	  }
	  if (admin)
	  {
		  admin = false;
		  HTMLvars.init(pageStor.at(16).getBody());

		  switch (atoi(r->params_["page"].c_str()))
		  {
		  case 0:
			  HTMLvars.variables["aloc"] = "Home";
			  HTMLvars.variables["adat"] = "<p>Welcome to the BitBet admin panel home page!</p>";
			  break;

		  case 1:
			  if (r->params_["ban"] != "NOBAN")
			  {
				  for (auto& Account : authenticationStorage)
				  {
					  if (r->params_["ban"] == Account.getUsername())
					  {
						  if (Account.isBanned())
							  Account.ban();
						  else
							  Account.unban();
						  break;
					  }
				  }
			  }
			  HTMLvars.variables["aloc"] = "Account Management";
			  HTMLvars.variables["adat"] = "<p>Welcome to the account management page</p>"
				  "<p>" + adminPan.listAccounts(r->params_["session"]) + "</p>";
			  break;

		  case 2:
			  if (r->params_["newround"] == "true")
			  {
				  entryVars.init(pageStor.at(21).getBody());
				  entryVars.variables["sess"] = r->params_["session"];
				  HTMLvars.variables["aloc"] = "Create new round";
				  HTMLvars.variables["adat"] = entryVars.writeVars();
				  entryVars.reset();
			  }
			  else if (r->params_["cnewround"] == "true")
			  {
				  defaultBitBet.newRound(r->params_["roundName"], atof(r->params_["startingPrice"].c_str()), atof(r->params_["growth"].c_str()),
					  seedsalt, atoi(r->params_["runningTime"].c_str()));
				  HTMLvars.variables["aloc"] = "Round Created!";
				  HTMLvars.variables["adat"] = "The round " + r->params_["roundName"] + " has been successfully created!";
			  }
			  else
			  {
				  HTMLvars.variables["aloc"] = "Round Management";
				  for (auto& Round : defaultBitBet.getRounds())
				  {
					  entryVars.init(pageStor.at(14).getBody());
					  entryVars.variables["rnam"] = Round.getName();
					  entryVars.variables["ratt"] = Round.getRoundAttributes();
					  entryVars.variables["sess"] = r->params_["session"];
					  entryVars.variables["roid"] = Round.getRoundID();
					  HTMLvars.variables["adat"] += entryVars.writeVars();
					  entryVars.reset();
				  }
				  HTMLvars.variables["adat"] += "<p>Click <a href='/12?newround=true&session=" + r->params_["session"] + "&page=2'>here</a> to create a new round!";
			  }
			  break;

		  default:
			  break;
		  }

		  HTMLvars.variables["sess"] = r->params_["session"];
		  HTMLvars.variables["user"] = curAcc;
		  HTMLvars.variables["usr1"] = curAcc;
		  HTMLvars.variables["ses0"] = r->params_["session"];
		  HTMLvars.variables["ses1"] = r->params_["session"];
		  HTMLvars.variables["ses2"] = r->params_["session"];
		  HTMLvars.variables["ses3"] = r->params_["session"];
		  r->answer_ = HTMLvars.writeVars();
		  HTMLvars.reset();
	  }
	  else
	  {
		  r->answer_ = pageStor.at(1).getBody();
	  }

	  curAcc = "";
	  

	  r->params_.clear();
	  break;


  case 400:
	  r->answer_ = pageStor.at(18).getBody();

	  r->params_.clear();
	  break;

  case 401:
	  r->answer_ = pageStor.at(19).getBody();

	  r->params_.clear();
	  break;

  case 1000: //CSS page series(1000-1999)
	  r->content_type_ = "text/css";
	  r->answer_ = pageStor.at(5).getBody();//get CSS foundation sheet

	  r->params_.clear();
	  break;

  case 1001:
	  r->content_type_ = "text/css";
	  r->answer_ = pageStor.at(17).getBody();//get CSS foundation admin sheet

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
	  body = "404, page not found! click <a href='/0'>here</a> to return to the home page.";
	  title = body;
	  r->answer_ = getAnswer(body, title, bgcolor);
	  r->params_.clear();
	  break;
  }

  
  
}

int main() {
	load.loadAuthorisations();
	setPresets(&pageStor, &currentSessionID);
	std::cout << "attempting to start the server on port 80" << std::endl;
	webserver(80, Get);

}
