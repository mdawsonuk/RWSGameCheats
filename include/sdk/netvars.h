#pragma once

#include <string>


// This macro defines a function called name which returns type
//		The function gets the netvar offset from the strings tableName and varName
//		Then it takes the address of the class (from this), adds the offset, and casts the result to type
#define NETVAR(name, type, tableName, varName) \
type& name##() \
{ \
	static int offset = Netvars::GetOffset(tableName, varName); \
	return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
}

#define NETVAR_ARRAY(name, type, tableName, varName) \
type* name##() \
{ \
	static int offset = Netvars::GetOffset(tableName, varName); \
	return reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
}

namespace Netvars
{

	bool SetupNetvars();

	void DumpTables();
	
	int GetOffset(std::string tableName, std::string varName);
}