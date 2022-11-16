#pragma once

#include <string>
#include <vector>

#include "sdk/common.h"

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

class CRecvProxyData;
typedef void (*RecvProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);

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
	RecvProxyFn m_ProxyFn;
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

class DVariant
{
public:
	union
	{
		float	m_Float;
		long	m_Int;
		char* m_pString;
		void* m_pData;	// For DataTables.
		float	m_Vector[3];
		int64	m_Int64;
	};
	SendPropType	m_Type;
};

class CRecvProxyData
{
public:
	const RecvProp* m_pRecvProp;		// The property it's receiving.

	DVariant		m_Value;			// The value given to you to store.

	int				m_iElement;			// Which array element you're getting.

	int				m_ObjectID;			// The object being referred to.
};


namespace Netvars
{
	bool SetupNetvars();

	void DumpTables();
	
	int GetOffset(std::string tableName, std::string varName);

	RecvProxyFn HookRecvProxy(std::string tableName, std::string varName, RecvProxyFn hookFn);

	void UnhookAllRecvProxies();
}