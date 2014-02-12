#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "HTMLVariables.h"


HTMLVariables::HTMLVariables(std::string file)
{
	for (int i = 0; i < file.size(); i++)
	{
		if (file.at(i) == '^')
		{
			for (int x = i; x < file.size(); x++)
			{
				if (file.at(x) == '^')
				{
					positions.emplace_back(htmlvars::intpair(i, x));
					variables[file.substr(i + 1, x - 1)] = "";
					std::cout << "HTMLVARIABLES found variable: " << file.substr(i + 1, x - 1) << std::endl;
					i = x + 1;
					break;
				}
			}
		}
	}
}

HTMLVariables::HTMLVariables()
{

}

void HTMLVariables::reset()
{
	filecontents.clear();
	positions.clear();
	variables.clear();
	variableList.clear();
}

void HTMLVariables::init(std::string file)
{
	int pospos = 0;
	filecontents = file;
	std::string tmp;
	char denim = '^';
	std::cout << "entered htmlvars init()\n";
	for (int i = 0; i < file.size(); i++)
	{
		if (file.at(i) == denim)
		{

			positions.emplace_back(htmlvars::intpair(i, pospos)); pospos++;
				
				for (int x = 1; x < 5; x++)
				{
					tmp += file.at(i + x);
				}
				variables[tmp] = "";
				std::cout << "HTMLVariables got: " << tmp << std::endl;
				variableList.emplace_back(tmp);
				tmp.clear();
		}
	}
	std::cout << "leaving htmlvars init()\n";
}

std::string HTMLVariables::writeVars()
{
	int varsDone = 1;
	char denim = '^';
	for (int i = 0; i < variableList.size(); i++)//TODO after every loop redetect variable positions.
	{
		filecontents.erase(positions.at(i).a, 5);
		filecontents.insert(positions.at(i).a, variables[variableList.at(i)]);

		//redetect var positions here

		for (int x = positions.at(i).a; x < filecontents.size(); x++)
		{
			if (filecontents.at(x) == denim)
			{
				if (positions.size() < i + varsDone) break;
				positions.at(i + varsDone).a = x;
				varsDone++;
			}
		}
		varsDone = 1;


	}
	return filecontents;
}

HTMLVariables::~HTMLVariables()
{
}
