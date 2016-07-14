#pragma once
#include "IVehicle.h"
#include "IPerson.h"

class IBus : public IVehicle<IPerson>
{
};
