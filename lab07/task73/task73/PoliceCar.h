#pragma once
#include "CarImpl.h"
#include "IPoliceCar.h"
#include "IPoliceMan.h"

class CPoliceCar : public CCarImpl<CVehicleImpl<IPoliceCar, IPoliceMan>>
{
public:
	CPoliceCar(MakeOfTheCar brand, size_t placeCount);
};
