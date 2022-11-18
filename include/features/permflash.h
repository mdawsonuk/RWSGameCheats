#pragma once

#include "sdk/interfaces.h"
#include "sdk/netvars.h"

namespace PermFlash
{
	void OnFramePostDataUpdateStart();

	extern RecvProxyFn ogRecvProxy_FlashTime;
	void RecvProxy_FlashTime(const CRecvProxyData* pData, void* pStruct, void* pOut);
}