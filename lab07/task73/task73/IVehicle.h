#pragma once
#include "IBasicVehicle.h"

template<class Passenger>
class IVehicle : public IBasicVehicle
{
public:
	virtual void AddPassenger(std::shared_ptr<Passenger> const & passenger) = 0;
	virtual Passenger const & GetPassenger(size_t index) const = 0;
	virtual void RemovePassenger(size_t index) = 0;
};
