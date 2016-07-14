#pragma once
#include "CarImpl.h"
#include "IRacingCar.h"
#include "IRacer.h"

class CRacingCar : public CCarImpl<CVehicleImpl<IRacingCar, IRacer>>
{
public:
	CRacingCar(MakeOfTheCar brand, size_t placeCount);
};
