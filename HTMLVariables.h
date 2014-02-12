#pragma once

namespace htmlvars
{
	class intpair
	{
	public:
		intpair()
		{

		}
		intpair(int _a, int _b)
		{
			a = _a;
			b = _b;
		}
		~intpair()
		{

		}
		int a, b;
	};
}

class HTMLVariables
{
private:
	std::vector<htmlvars::intpair> positions;
	std::string filecontents;
public:
	void reset();
	std::vector<std::string> variableList;
	HTMLVariables(std::string file);
	HTMLVariables();
	void init(std::string file);
	std::map<std::string, std::string> variables;
	std::string writeVars();
	~HTMLVariables();
};

