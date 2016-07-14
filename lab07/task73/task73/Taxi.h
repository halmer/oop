#pragma once
#include "CarImpl.h"
#include "ITaxi.h"
#include "IPerson.h"

class CTaxi : public CCarImpl<CVehicleImpl<ITaxi, IPerson>>
{
public:
	CTaxi(MakeOfTheCar brand, size_t placeCount);
};
