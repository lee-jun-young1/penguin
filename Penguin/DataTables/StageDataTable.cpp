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

bool StageDataTable::Load()
{
	std::string filename = "data/StageData.csv";

	rapidcsv::Document doc(filename);
	std::vector<float> distance = doc.GetColumn<float>(0);
	std::vector<float> timeLimit = doc.GetColumn<float>(1);

	for (int j = 0; j < distance.size(); j++)
	{
		std::cout << distance[j] << " " << timeLimit[j] << std::endl;
		table.push_back({ distance[j], timeLimit[j] });
	}
	return true;
}

void StageDataTable::Release()
{
	table.clear();
	//tables.clear();
}
