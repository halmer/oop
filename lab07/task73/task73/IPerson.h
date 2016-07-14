#pragma once

class IPerson
{
public:
	virtual std::string GetName() const = 0;
	virtual ~IPerson(){}
};
