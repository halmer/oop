#pragma once
#include "ICar.h"
#include "IPoliceMan.h"

class IPoliceCar : public ICar<IPoliceMan>
{
};
