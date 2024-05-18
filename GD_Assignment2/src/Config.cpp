#include "Config.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

static const std::string to_lower(std::string s) {
	for (char& c : s)
		c = std::tolower(c);
	return s;
}

Config::Config(std::string filePath)
{
	std::ifstream configFile(filePath, std::ios::in);
	std::string line;
	while (std::getline(configFile, line))
	{
		std::stringstream ssLine(line);
		std::string token;
		ssLine >> token;
		token = to_lower(token);

		if (token == "window")
		{
			ssLine >> windowWidth >> windowHeight >> windowFPS >> windowFullScreen;
		}
		else if (token == "font")
		{
			ssLine >> fontFilePath >> fontSize >> fontR >> fontG >> fontB;

		}
		else if (token == "player")
		{
			ssLine 
				>> playerSR 
				>> playerCR 
				>> playerSpeed 
				>> playerFR >> playerFG >> playerFB 
				>> playerOR >> playerOG >> playerOB 
				>> playerOT 
				>> playerVertices;
		}
		else if (token == "enemy")
		{
			ssLine
				>> enemySR
				>> enemyCR
				>> enemySMin >> enemySMax
				>> enemyOR >> enemyOG >> enemyOB
				>> enemyOT
				>> enemyVertMin >> enemyVertMax
				>> enemyL
				>> enemySI;
		}
		else if (token == "bullet")
		{
			ssLine
				>> bulletSR
				>> bulletCR
				>> bulletSpeed
				>> bulletFR >> bulletFG >> bulletFB
				>> bulletOR	>> bulletOG >> bulletOB
				>> bulletOT
				>> bulletVertices
				>> bulletLifespan;
		}
		else
		{
			std::cerr << "Unknown setting name token found : " << token << ", in file " << filePath << std::endl;
		}
	}
}
