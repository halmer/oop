#pragma once
#include "Body.h"

class CCompound final : public CBody
{
public:
	CCompound(std::string const & id);

	double GetDensity() const override;
	double GetVolume() const override;
	double GetMass() const override;
	bool AddBody(std::shared_ptr<CBody> const & body);
	bool RemoveBody(std::shared_ptr<CBody> const & body);

protected:
	void AppendProperties(std::ostream & strm) const override;

private:
	bool IsContainedIn(std::shared_ptr<CBody> const & body) const;

	std::set<std::shared_ptr<CBody>> m_bodies;
};
