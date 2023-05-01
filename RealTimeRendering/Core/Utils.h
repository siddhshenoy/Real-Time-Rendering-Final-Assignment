#pragma once

#include <iostream>
#include <string>
#include <fstream>

class Utils
{
public:
	static void ReadFullFile(const char* filePath, std::string& file_data);
	static void InitRandomSeed();
	static int GenerateRandom(int minX, int maxY);
	static float GenerateRandomFloat(int minX, int maxY);
};

