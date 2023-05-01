#include "Utils.h"

void Utils::ReadFullFile(const char* filePath, std::string& file_data)
{
	std::fstream file;
	file.open(filePath, std::fstream::in);
	if (file.good()) {
		std::string line;
		file_data = "";
		while (!file.eof()) {
			std::getline(file, line);
			file_data += line + "\r\n";
		}
		file.close();

	}
}

void Utils::InitRandomSeed()
{
	srand(time(0));
}

int Utils::GenerateRandom(int min, int max)
{

	return ((rand() % (max + 1)) + min);
}

float Utils::GenerateRandomFloat(int min, int max)
{
	int result = GenerateRandom(min, max);
	//std::cout << __FUNCTION__ << ": " << result << std::endl;
	float fResult = static_cast<float>(result) / static_cast<float>(max);
	//std::cout << "fResult: " << fResult << std::endl;
	fResult = (float)(max - min) * (float)(fResult);
	//std::cout << "final fResult: " << fResult << std::endl;
	return fResult;
}
