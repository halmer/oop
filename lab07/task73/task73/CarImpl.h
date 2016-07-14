#pragma once
#include "ICar.h"
#include "VehicleImpl.h"

template <class Base>
class CCarImpl : public Base
{
public:
	CCarImpl(MakeOfTheCar brand, size_t placeCount)
		: Base(placeCount)
		, m_brand(brand)
	{}

	MakeOfTheCar GetMakeOfTheCar() const override
	{
		return m_brand;
	}

private:
	MakeOfTheCar m_brand;
};
