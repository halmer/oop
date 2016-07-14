#pragma once
#include "VehicleImpl.h"
#include "IBus.h"
#include "IPerson.h"

class CBus : public CVehicleImpl<IBus, IPerson>
{
public:
	explicit CBus(size_t placeCount);
};
