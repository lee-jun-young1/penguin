#include "stdafx.h"
#include "DataTableManager.h"
#include "StringTable.h"
#include "StageDataTable.h"

void DataTableManager::LoadAll()
{
	tables.insert({ DataTable::Ids::String, new StringTable() });
	tables.insert({ DataTable::Ids::Stage, new StageDataTable() });

	for (auto pair : tables)
	{
		if (!pair.second->Load())
		{
			std::cout << "ERR:: Data Table Load Fail" << std::endl;
			exit(-1);
		}
	}
}

void DataTableManager::ReleaseAll()
{
	for (auto pair : tables)
	{
		delete pair.second;
	}
	tables.clear();
}
