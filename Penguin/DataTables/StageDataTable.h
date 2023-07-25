#pragma once
#include "DataTable.h"

struct StageData
{
	float distance;
	float timeLimit;
	std::string name;
	sf::Color skyColor;
};

class StageDataTable : public DataTable
{
protected:
	std::vector <StageData> table;
public:
	StageDataTable() : DataTable(DataTable::Ids::Stage) {}
	virtual ~StageDataTable() override { Release(); }

	const float& GetDistance(const int& index) const;
	const float& GetTimeLimit(const int& index) const;

	const std::string& GetStageName(const int& index) const;
	const sf::Color& GetSkyColor(const int& index) const;

	// DataTable을(를) 통해 상속됨
	virtual bool Load() override;
	virtual void Release() override;
};

