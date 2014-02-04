#pragma once
class FileIO
{
private:
	std::ofstream Out;
	std::ifstream In;
	std::string fileName;
	std::vector<std::string> fileContents;
	std::string _tmp;
	void loadFile()
	{
		if (In.fail())
		{
			std::cout << "File not found...\nCreating new file.\n";
			Out.close();
			Out.open(fileName);
		}
		else
		{
			while (!In.eof())
			{
				In >> _tmp;
				fileContents.emplace_back(_tmp);
			}
			std::cout << "File successfully loaded!\n";
		}
	}

public:
	FileIO(std::string filename)
	{
		fileName = filename;
		Out.open(filename + "_TMP");
		In.open(filename);
		loadFile();
	}

	FileIO()
	{

	}

	void init(std::string filename)
	{
		fileName = filename;
		Out.open(filename + "_TMP");
		In.open(filename);
		loadFile();
	}

	void initNoInput(std::string filename)
	{
		fileName = filename;
		Out.open(filename);
	}

	void writeInt(int write)
	{
		Out << std::to_string(write) << std::endl;
	}

	void writeString(std::string str)
	{
		Out << str << std::endl;
	}

	void writeScore(std::string name, int score)
	{
		Out << name << ":" << std::to_string(score) << std::endl;
	}

	void closeFile()
	{
		std::string _tmpstr;
		In.close();
		Out.close();
		In.open(fileName + "_TMP");
		Out.open(fileName);
		while (!In.eof())
		{
			In >> _tmpstr;
			Out << _tmpstr;
			_tmpstr = "";
		}
		In.close();
		Out.close();
	}

	bool lineExisting(int num)
	{
		if (num < fileContents.size())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int amountOfLines()
	{
		return fileContents.size();
	}

	//This reads the line num from the vector, always call lineExisting() before calling this or you might get memory access violations
	std::string readLine(int num)
	{
		return fileContents.at(num);
	}
};

