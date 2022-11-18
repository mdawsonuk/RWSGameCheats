#include "features/permflash.h"
#include "features/settings.h"

#include "sdk/classes/C_BasePlayer.h"

namespace PermFlash
{
	RecvProxyFn ogRecvProxy_FlashTime = nullptr;

	int lastPlayerDeaths = 0;


	void RecvProxy_FlashTime(const CRecvProxyData* pData, void* pStruct, void* pOut)
	{
		auto data = const_cast<CRecvProxyData*>(pData);
		if (Settings::isPermFlash) {
			printf_s("FlashTime: %f\n", pData->m_Value.m_Float);
			// 3.40282e+038 is float max value
			data->m_Value.m_Float = 300.0f;
		}

		ogRecvProxy_FlashTime(pData, pStruct, pOut);
	}

	void OnFramePostDataUpdateStart() {
		int localPlayerIndex = g_EngineClient->GetLocalPlayer();
		auto csPlayerResource = *g_CSPlayerResource;
		auto localPlayer = *g_LocalPlayer;

		if (!csPlayerResource || !localPlayer) {
			return;
		}

		int playerDeaths = csPlayerResource->m_iDeaths()[localPlayerIndex];

		if (playerDeaths != lastPlayerDeaths) {
			CRecvProxyData* data = new CRecvProxyData;
			data->m_Value = DVariant();
			data->m_Value.m_Float = 0.0f;
			lastPlayerDeaths = playerDeaths;
			localPlayer->m_flFlashDuration() = 0.0f;
			ogRecvProxy_FlashTime(data, localPlayer, NULL);
		}
	}
}