#include "stdafx.h"
#include "StageDataTable.h"
#include "rapidcsv.h"

const float& StageDataTable::GetDistance(const int& index) const
{
	return table[index].distance;
}

const float& StageDataTable::GetTimeLimit(const int& index) const
{
	return table[index].timeLimit;
}

const std::string& StageDataTable::GetStageName(const int& index) const
{
	return table[index].name;
}

const sf::Color& StageDataTable::GetSkyColor(const int& index) const
{
	return table[index].skyColor;
}


bool StageDataTable::Load()
{
	std::string filename = "data/StageData.csv";

	rapidcsv::Document doc(filename);
	std::vector<float> distance = doc.GetColumn<float>(0);
	std::vector<float> timeLimit = doc.GetColumn<float>(1);
	std::vector<std::string> name = doc.GetColumn<std::string>(2);
	std::vector<int> skyR = doc.GetColumn<int>(3);
	std::vector<int> skyG = doc.GetColumn<int>(4);
	std::vector<int> skyB = doc.GetColumn<int>(5);

	for (int j = 0; j < distance.size(); j++)
	{
		std::cout << distance[j] << " " << timeLimit[j] << " " << name[j] << std::endl;
		table.push_back({ distance[j], timeLimit[j], name[j], sf::Color(skyR[j], skyG[j], skyB[j]) });
	}
	return true;
}

void StageDataTable::Release()
{
	table.clear();
}
