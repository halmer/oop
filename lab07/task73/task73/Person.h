#pragma once
#include "PersonImpl.h"
#include "IPerson.h"

class CPerson : public CPersonImpl<IPerson>
{
public:
	explicit CPerson(std::string const & name);
};
