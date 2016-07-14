#pragma once
#include "ICar.h"
#include "IPerson.h"

class ITaxi : public ICar<IPerson>
{
};
