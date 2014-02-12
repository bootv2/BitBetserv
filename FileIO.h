#pragma once
class FileIO
{
private:
	std::ofstream* Out;
	std::ifstream In;
	std::string fileName;
	std::vector<std::string> fileContents;
	std::string _tmp;
	std::string filePath;
	bool testbool = true;
	void loadFile()
	{
		if (htmlFile)
		{
			fileContents.clear();
			if (In.fail())
			{
				std::cout << "File not found...\nCreating new file.\n";
				Out->close();
				delete Out;

			}
			else
			{
				std::cout << "html file loading...\n";
				while (!In.eof())
				{
					In >> _tmp;
					
					_tmp = _tmp + " ";

					fileContents.emplace_back(_tmp);

					_tmp.clear();
					
					
				}

				std::cout << "File successfully loaded!\n";
			}
		}

		else
		{
			fileContents.clear();
			if (In.fail())
			{
				std::cout << "File not found...\nCreating new file.\n";
				Out->close();
				delete Out;
				Out = new std::ofstream();
				Out->open(fileName);
			}
			else
			{
				while (!In.eof())
				{
					In >> _tmp;
					for (int i = 0; i < split(_tmp, '&').size(); i++)
					{
						fileContents.emplace_back(split(_tmp, '&').at(i));
						std::cout << fileContents.at(i) << std::endl;
					}
				}
				std::cout << "File successfully loaded!\n";
			}
		}
	}

	std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems)
	{
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	std::string getSource(std::string input)
	{
		std::string ret;
		if (!find(input, "src=\"")) return "";
		std::cout << input;
		int startpos = findpos(input, '\"') + 1;
		for (int i = startpos; i < findpos(input, '\"', startpos + 1); i++)
		{
			ret += input.at(i);
			if (i == input.size()) return ret;
		}
		return ret;
	}

	bool find(std::string input, std::string findstr)
	{
		std::string check = "";
		for (int i = 0; i < input.size(); i++)
		{
			for (int x = i; x < (i + findstr.size()) && x < input.size(); x++)
			{
				check += input.at(x);
			}
			if (check.compare(findstr) == 0) return true;
			check.clear();
		}
		return false;
	}

	int findpos(std::string input, char find)
	{
		for (int i = 0; i < input.size(); i++)
		{
			if (input.at(i) == find) return i;
		}
		return -1;
	}

	int findpos(std::string input, char find, int startPos)
	{
		for (int i = startPos; i < input.size(); i++)
		{
			if (input.at(i) == find) return i;
		}
		return -1;
	}
	
	bool htmlFile = false;

public:
	FileIO(std::string filename)
	{	
		Out = new std::ofstream();
		fileName = filename;
		Out->open(filename + "_TMP");
		In.open(filename);
		loadFile();
	}

	FileIO(std::string _filePath, std::string filename, bool _html)
	{
		filePath = _filePath;
		htmlFile = _html;
		fileName = filename;
		In.open(filePath + filename);
		loadFile();
	}

	FileIO()
	{

	}

	void openFile()
	{
		delete Out;
		Out = new std::ofstream();
		Out->open(fileName + "_TMP");
		In.open(fileName);
		loadFile();
	}

	void init(std::string filename)
	{
		Out = new std::ofstream();
		fileName = filename;
		Out->open(filename + "_TMP");
		In.open(filename);
		loadFile();
	}

	void initHTML(std::string _path, std::string _filename)
	{
		In.open(_path + _filename);
		filePath = _path;
		fileName = _filename;
		loadFile();

	}

	void initNoInput(std::string filename)
	{
		Out = new std::ofstream();
		fileName = filename;
		Out->open(filename);
	}

	void writeInt(int write)
	{
		*Out << std::to_string(write) << std::endl;
	}

	void writeString(std::string str)
	{
		*Out << str;
	}

	void writeScore(std::string name, int score)
	{
		*Out << name << ":" << std::to_string(score) << std::endl;
	}

	void closeStream()
	{
		delete Out;
	}

	void closeFile()
	{
		if (!htmlFile)
		{
			std::string _tmpstr;
			In.close();
			Out->close();
			In.open(fileName + "_TMP");
			delete Out;
			Out = new std::ofstream();
			Out->open(fileName);
			while (!In.eof())
			{
				In >> _tmpstr;
				*Out << _tmpstr;
				_tmpstr = "";
			}
			Out->close();
			delete Out;
		}
		In.close();
		fileContents.clear();
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

