#include "sdk\netvars.h"

#include <stdio.h>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "sdk\interfaces.h"

namespace Netvars
{
	struct Table;
	using TablePtr = std::shared_ptr<Table>;

	struct Table
	{
		std::string name;
		int offset = 0;

		std::map<std::string, RecvProp*> props;
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
			auto prop = &recvTable->m_pProps[i];

			// Hide the spam of array members
			if (isdigit(prop->m_pVarName[0]))
			{
				continue;
			}

			if (prop->m_RecvType == DPT_DataTable && prop->m_pDataTable)
			{
				auto childTable = SetupTable(prop->m_pDataTable);
				childTable->offset = prop->m_Offset;
				table->childTables.push_back(childTable);
			}
			else
			{
				table->props.emplace(prop->m_pVarName, prop);
			}
		}

		return table;
	}

	void DumpTable(FILE* output, TablePtr table, int indent)
	{
		for (auto iter = table->props.begin(); iter != table->props.end(); ++iter)
		{
			auto& prop = *iter;
			fprintf_s(output, "%*c%-50s: 0x%X\n", (indent + 1) * 4, ' ', prop.first.c_str(), prop.second->m_Offset + table->offset);
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
			return table->offset + table->props.at(varName)->m_Offset;
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
		DumpTables();

		return true;
	}

	struct RecvProxyHook
	{
		RecvProp* prop;
		RecvProxyFn ogFn;
	};
	std::vector<RecvProxyHook> hookedRecvProxies;

	RecvProxyFn HookRecvProxy(std::string tableName, std::string varName, RecvProxyFn hookFn)
	{
		for (auto& table : tables)
		{
			if (table->name == tableName)
			{
				if (table->props.find(varName) != table->props.end())
				{
					auto prop = table->props.at(varName);

					auto ogFn = prop->m_ProxyFn;
					prop->m_ProxyFn = hookFn;

					RecvProxyHook hook = { 0 };
					hook.prop = prop;
					hook.ogFn = ogFn;
					hookedRecvProxies.push_back(hook);

					return ogFn;
				}
				break;
			}
		}
		return nullptr;
	}

	void UnhookAllRecvProxies()
	{
		for (auto& hook : hookedRecvProxies)
		{
			hook.prop->m_ProxyFn = hook.ogFn;
		}
	}

}