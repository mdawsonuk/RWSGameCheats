#pragma once

#include "sdk/interfaces.h"

namespace Chams
{

	void OnDrawModelExecute(IVModelRender* _this, IMatRenderContext* pRenderContext, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);

}