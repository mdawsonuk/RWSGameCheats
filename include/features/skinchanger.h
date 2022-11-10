#pragma once

#include "sdk/interfaces.h"
#include "sdk/netvars.h"

namespace SkinChanger
{

	// TODO: Hook frame stage notify

	// TODO: Hook DT_BaseViewModel m_nModelIndex
	// TODO: Hook DT_BaseViewModel m_nSequence

	void OnFramePostDataUpdateStart();

	extern RecvProxyFn ogRecvProxy_nModelIndex;
	void RecvProxy_nModelIndex(const CRecvProxyData* pData, void* pStruct, void* pOut);

	extern RecvProxyFn ogRecvProxy_nSequence;
	void RecvProxy_nSequence(const CRecvProxyData* pData, void* pStruct, void* pOut);

	void DumpModelIndices();
}