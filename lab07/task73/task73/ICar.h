#pragma once
#include "IVehicle.h"

enum class MakeOfTheCar
{
	Acura,
	Audi,
	BMW,
	Honda,
	Mazda,
	Mercedes,
	Mitsubishi,
	Nissan,
	Opel,
	Porsche,
	Subaru,
	Suzuki,
	Toyota,
	Volkswagen,
};

template <class Passenger>
class ICar : public IVehicle<Passenger>
{
public:
	virtual MakeOfTheCar GetMakeOfTheCar() const = 0;
};
