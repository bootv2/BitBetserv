#pragma once
class LoadingClass
{
private:
	FileIO loadingIO;
	BitBet* bb;
	std::string authFileName = "authorisation.dbs";
	std::string bbFileName = "bbGameFile.dbs";
	std::vector<std::string> lines;
	std::vector<Account>* pAuthStorage;
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
public:
	LoadingClass(std::vector<Account>* _pAuthStor, BitBet* _bb);
	void loadAuthorisations();
	void saveAuthorisations();
	void saveBitBet();
	~LoadingClass();
};

