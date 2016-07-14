#pragma once

template <class Base, class Passenger>
class CVehicleImpl : public Base
{
public:
	CVehicleImpl(size_t placeCount)
		: m_placeCount(placeCount)
	{}

	bool IsEmpty() const override
	{
		return m_passengers.empty();
	}

	bool IsFull() const override
	{
		return m_passengers.size() == m_placeCount;
	}

	size_t GetPlaceCount() const override
	{
		return m_placeCount;
	}

	size_t GetPassengerCount() const override
	{
		return m_passengers.size();
	}

	void RemoveAllPassengers() override
	{
		m_passengers.clear();
	}

	void AddPassenger(std::shared_ptr<Passenger> const & passenger) override
	{
		if (IsFull())
		{
			throw std::logic_error("No free places");
		}

		m_passengers.push_back(passenger);
	}

	Passenger const & GetPassenger(size_t index) const override
	{
		return *m_passengers.at(index);
	}

	void RemovePassenger(size_t index) override
	{
		if (index >= m_passengers.size())
		{
			throw std::out_of_range("Invalid index");
		}

		m_passengers.erase(m_passengers.begin() + index);
	}

private:
	std::vector<std::shared_ptr<Passenger>> m_passengers;
	size_t m_placeCount;
};
