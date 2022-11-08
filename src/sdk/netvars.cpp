#include "sdk\netvars.h"

#include <stdio.h>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "sdk\interfaces.h"

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
	DPT_String,
	DPT_Array,	// An array of the base types (can't be of datatables).
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
} SendPropType;

class RecvTable;

class RecvProp
{
public:
	const char* m_pVarName;
	int m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	void* m_pExtraData;
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxyFn;
	void* m_ProxyFn;
	void* m_DataTableProxyFn;
	RecvTable* m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char* m_pParentArrayPropname;
};

class RecvTable
{
public:
	RecvProp* m_pProps;
	int m_nProps;
	void* m_pDecoder;
	const char* m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

class ClientClass
{
public:
	void* m_pCreateFn;
	void* m_pCreateEventFn;
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassId;
	const char* m_pMapClassname;
};

namespace Netvars
{
	struct Table;
	using TablePtr = std::shared_ptr<Table>;

	struct Table
	{
		std::string name;
		int offset = 0;

		std::map<std::string, int> props;
		std::vector<TablePtr> childTables;
	};
	std::vector<TablePtr> tables;

	TablePtr SetupTable(RecvTable* recvTable)
	{
		auto table = TablePtr(new Table);
		table->name = recvTable->m_pNetTableName;
		table->offset = 0;

		for (int i = 0; i < recvTable->m_nProps; i++)
		{
			auto prop = recvTable->m_pProps[i];

			// Hide the spam of array members
			if (isdigit(prop.m_pVarName[0]))
			{
				continue;
			}

			if (prop.m_RecvType == DPT_DataTable && prop.m_pDataTable)
			{
				auto childTable = SetupTable(prop.m_pDataTable);
				childTable->offset = prop.m_Offset;
				table->childTables.push_back(childTable);
			}
			else
			{
				table->props.emplace(prop.m_pVarName, prop.m_Offset);
			}
		}

		return table;
	}

	void DumpTable(FILE* output, TablePtr table, int indent)
	{
		for (auto iter = table->props.begin(); iter != table->props.end(); ++iter)
		{
			auto& prop = *iter;
			fprintf_s(output, "%*c%-50s: 0x%X\n", (indent + 1) * 4, ' ', prop.first.c_str(), prop.second + table->offset);
		}

		for (auto& childTable : table->childTables)
		{
			fprintf_s(output, "%*c%-50s: 0x%X\n", (indent + 1) * 4, ' ', childTable->name.c_str(), childTable->offset + table->offset);
			DumpTable(output, childTable, indent + 1);
		}
	}

	void DumpTables()
	{
		// Let's dump this to a file instead of the console....
		FILE* output;
		fopen_s(&output, "netvar_dump.txt", "w");
		if (!output)
		{
			return;
		}

		for (auto& table : tables)
		{
			fprintf_s(output, "%-50s\n", table->name.c_str());
			DumpTable(output, table, 0);
		}

		fclose(output);
	}

	int GetOffset(TablePtr table, std::string& varName)
	{
		// Check if this table has the variable
		if (table->props.find(varName) != table->props.end())
		{
			return table->offset + table->props.at(varName);
		}

		// If it doesn't, check the child tables
		for (auto& childTable : table->childTables)
		{
			int offset = GetOffset(childTable, varName);
			if (offset)
			{
				return table->offset + offset;
			}
		}

		// Are we maybe looking for a table itself? (e.g. one of the bitmasks)
		for (auto& childTable : table->childTables)
		{
			if (childTable->name == varName)
			{
				return table->offset + childTable->offset;
			}
		}

		// not found ):
		return 0; 
	}

	int GetOffset(std::string tableName, std::string varName)
	{
		for (auto& table : tables)
		{
			if (table->name == tableName)
			{
				return GetOffset(table, varName);
			}
		}

		// not found ):
		return 0; 
	}

	bool SetupNetvars()
	{
		auto head = g_ClientDLL->GetAllClasses();

		// Setup all tables
		for (ClientClass* cur = head; cur != nullptr; cur = cur->m_pNext)
		{
			if (cur->m_pRecvTable)
			{
				tables.push_back(SetupTable(cur->m_pRecvTable));
			}
		}

		// TODO: Uncomment this if you want to dump all tables
		//			they will be in your csgo directory in the netvar_dump.txt 
		// 
		//DumpTables();

		return true;
	}
}