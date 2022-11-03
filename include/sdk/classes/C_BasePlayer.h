#pragma once

#include "C_BaseEntity.h"


// There is actually a long inheritance chain between C_BasePlayer and C_BaseEntity
// but for what we need to do, this is enough (I also don't think we care about the custom materials...)

class C_BasePlayer : public C_BaseEntity /*, public CCustomMaterialOwner*/
{
public:

	// TODO: Player netvars
};