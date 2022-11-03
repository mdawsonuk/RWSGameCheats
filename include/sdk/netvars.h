#pragma once

#include <string>
// TODO: Netvar manager

namespace Netvars
{

	bool SetupNetvars();

	void DumpTables();
	
	int GetOffset(std::string& tableName, std::string& varName);
}