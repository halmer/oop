#pragma once
#include "PersonImpl.h"
#include "IRacer.h"

class CRacer : public CPersonImpl<IRacer>
{
public:
	CRacer(std::string const & name, size_t awardsCount);
	size_t GetAwardsCount() const override;

private:
	size_t m_awardsCount;
};
