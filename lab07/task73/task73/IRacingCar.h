#pragma once
#include "ICar.h"
#include "IRacer.h"

class IRacingCar : public ICar<IRacer>
{
};
