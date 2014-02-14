#include <string>
#include <random>
#include <time.h>
#include <vector>
#include <array>

#include "Message.h"


Message::Message()
{
}

void Message::genRandMessageID(int &ssalt)
{
		std::string result = "";
		std::string availableChars = "abcdefghijklmnopqrstuvwxyz1234567890-_!";
		for (int i = 0; i < 25; i++)
		{
			srand(time(NULL) + ssalt + i);
			result += availableChars.at(rand() % (availableChars.size() - 1));
			ssalt++;
		}
		ID = result;
	
}

void Message::setMessageContent(std::string content)
{

}

std::string Message::filterMessageContent(std::string content)
{
	/*class Program // XML FILTER todo: convert from c# to c++
{
    static void Main(string[] args)
    {
        string contents = string.Empty;

        XmlDocument document = new XmlDocument();
        document.LoadXml("<outer>a<b>b</b>c<i>d</i>e<b>f</b>g</outer>");

        foreach(XmlNode child in document.DocumentElement.ChildNodes)
        {
            if (child.NodeType == XmlNodeType.Element)
            {
                contents += child.InnerText;
            }
        }

        Console.WriteLine(contents);

        Console.ReadKey();
    }
}*/
	std::string ret = "";
	std::vector<std::array<int, 2>> htmlTagLocations;
	std::array<int, 2> tmpTagLoc = {0, 0};
	for (int i = 0; i < content.size(); i++)
	{
		if (content.at(i) == '<')
		{
			for (int x = i; x < content.size(); x++)
			{
				if (content.at(x) == '>')//closing statement for html tag, delete all content between the statement tags.
				{
					tmpTagLoc[0] = i;
					tmpTagLoc[1] = x;
					htmlTagLocations.emplace_back(tmpTagLoc);
					i = x + 1;
					break;
				}
				if (content.at(x) == '<')//This wasnt a html statement, do not filter
				{
					i = x;
					break;
				}
			}
		}
	}

	int diff;

	//delete tags:
	for (int i = 0; i < htmlTagLocations.size(); i++)
	{
		diff = htmlTagLocations.at(i)[1] - htmlTagLocations.at(i)[0];
		//tag deletion
		content = content.substr(0, htmlTagLocations.at(i)[0]) + content.substr(htmlTagLocations.at(i)[1], content.size());
		for (int x = i; x < htmlTagLocations.size(); x++)//recalculate tag locations after tag deletion.
		{
			htmlTagLocations.at(x)[0] -= diff;
			htmlTagLocations.at(x)[1] -= diff;
		}
	}
	return content;
}

std::string Message::getFilteredMessageContent()
{
	return contents;
}

Message::~Message()
{
}
