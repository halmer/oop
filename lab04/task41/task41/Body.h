#pragma once

class CBody
{
public:
	CBody(std::string const & type, std::string const & id);
	virtual ~CBody();

	virtual double GetDensity() const = 0;
	virtual double GetVolume() const = 0;
	virtual double GetMass() const = 0;
	std::string ToString() const;

protected:
	virtual void AppendProperties(std::ostream & strm) const = 0;

private:
	std::string m_type;
	std::string m_id;
};
