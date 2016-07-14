#include "stdafx.h"
#include "../task73/Person.h"
#include "../task73/Racer.h"
#include "../task73/PoliceMan.h"
#include "../task73/Bus.h"
#include "../task73/Taxi.h"
#include "../task73/RacingCar.h"
#include "../task73/PoliceCar.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(Person)
	//человек
	BOOST_AUTO_TEST_CASE(person)
	{
		CPerson person("Person");
		BOOST_CHECK_EQUAL(person.GetName(), "Person");
	}
	//гонщик
	BOOST_AUTO_TEST_CASE(racer)
	{
		CRacer racer("Racer", 5);
		BOOST_CHECK_EQUAL(racer.GetName(), "Racer");
		BOOST_CHECK_EQUAL(racer.GetAwardsCount(), 5);
	}
	//полицейский
	BOOST_AUTO_TEST_CASE(policeman)
	{
		CPoliceMan policeMan("PoliceMan", "Department");
		BOOST_CHECK_EQUAL(policeMan.GetName(), "PoliceMan");
		BOOST_CHECK_EQUAL(policeMan.GetDepartmentName(), "Department");
	}
BOOST_AUTO_TEST_SUITE_END()

struct Person_
{
	shared_ptr<CPerson> person1 = make_shared<CPerson>("Person1");

	shared_ptr<CRacer> racer1 = make_shared<CRacer>("Racer1", 3);
	shared_ptr<CRacer> racer2 = make_shared<CRacer>("Racer2", 5);
	shared_ptr<CRacer> racer3 = make_shared<CRacer>("Racer3", 7);

	shared_ptr<CPoliceMan> policeMan1 = make_shared<CPoliceMan>("PoliceMan1", "Department");
	shared_ptr<CPoliceMan> policeMan2 = make_shared<CPoliceMan>("PoliceMan2", "Department");
	shared_ptr<CPoliceMan> policeMan3 = make_shared<CPoliceMan>("PoliceMan3", "Department");
};

BOOST_FIXTURE_TEST_SUITE(Vehicle, Person_)
	//Автобус
	BOOST_AUTO_TEST_SUITE(Bus)
		//изначально пуст
		BOOST_AUTO_TEST_CASE(initially_empty)
		{
			CBus bus(3);
			BOOST_CHECK(bus.GetPlaceCount() == 3);

			BOOST_CHECK(bus.IsEmpty());
			BOOST_CHECK(!bus.IsFull());
			BOOST_CHECK(bus.GetPassengerCount() == 0);
		}
		//можно посадить пассажира
		BOOST_AUTO_TEST_CASE(can_put_passenger)
		{
			CBus bus(3);
			bus.AddPassenger(person1);
			BOOST_CHECK(!bus.IsFull());
			BOOST_CHECK(bus.GetPassengerCount() == 1);

			bus.AddPassenger(racer1);
			bus.AddPassenger(policeMan1);
			BOOST_CHECK(bus.IsFull());
			BOOST_CHECK(bus.GetPassengerCount() == 3);

			BOOST_CHECK_THROW(bus.AddPassenger(racer2), logic_error);
			BOOST_CHECK(bus.GetPassengerCount() == 3);
		}
		//можно получить ссылку на пассажира
		BOOST_AUTO_TEST_CASE(can_get_reference_to_passenger)
		{
			CBus bus(3);
			bus.AddPassenger(person1);
			bus.AddPassenger(racer1);
			bus.AddPassenger(policeMan1);
		
			BOOST_CHECK_EQUAL(bus.GetPassenger(0).GetName(), "Person1");
			BOOST_CHECK_EQUAL(bus.GetPassenger(2).GetName(), "PoliceMan1");

			BOOST_CHECK_THROW(bus.GetPassenger(3).GetName(), out_of_range);
		}
		//можно высадить пассажира(пассажиров)
		BOOST_AUTO_TEST_CASE(can_drop_off_passenger_or_passengers)
		{
			CBus bus(3);
			bus.AddPassenger(person1);
			bus.AddPassenger(racer1);
			bus.AddPassenger(policeMan1);
			BOOST_CHECK(bus.GetPassengerCount() == 3);

			bus.RemovePassenger(0);
			BOOST_CHECK(bus.GetPassengerCount() == 2);

			BOOST_CHECK_THROW(bus.RemovePassenger(2), out_of_range);
			BOOST_CHECK(bus.GetPassengerCount() == 2);

			bus.RemoveAllPassengers();
			BOOST_CHECK(bus.GetPassengerCount() == 0);
			BOOST_CHECK(bus.IsEmpty());
		}
	BOOST_AUTO_TEST_SUITE_END()
	//Такси
	BOOST_AUTO_TEST_SUITE(Taxi)
		//изначально пусто
		BOOST_AUTO_TEST_CASE(initially_empty)
		{
			CTaxi taxi(MakeOfTheCar::Volkswagen, 3);
			BOOST_CHECK(taxi.GetMakeOfTheCar() == MakeOfTheCar::Volkswagen);
			BOOST_CHECK(taxi.GetPlaceCount() == 3);
			
			BOOST_CHECK(taxi.IsEmpty());
			BOOST_CHECK(!taxi.IsFull());
			BOOST_CHECK(taxi.GetPassengerCount() == 0);
		}
		//можно посадить пассажира
		BOOST_AUTO_TEST_CASE(can_put_passenger)
		{
			CTaxi taxi(MakeOfTheCar::Volkswagen, 3);
			taxi.AddPassenger(person1);
			BOOST_CHECK(!taxi.IsFull());
			BOOST_CHECK(taxi.GetPassengerCount() == 1);

			taxi.AddPassenger(racer1);
			taxi.AddPassenger(policeMan1);
			BOOST_CHECK(taxi.IsFull());
			BOOST_CHECK(taxi.GetPassengerCount() == 3);

			BOOST_CHECK_THROW(taxi.AddPassenger(racer2), logic_error);
			BOOST_CHECK(taxi.GetPassengerCount() == 3);
		}
		//можно получить ссылку на пассажира
		BOOST_AUTO_TEST_CASE(can_get_reference_to_passenger)
		{
			CTaxi taxi(MakeOfTheCar::Volkswagen, 3);
			taxi.AddPassenger(person1);
			taxi.AddPassenger(racer1);
			taxi.AddPassenger(policeMan1);

			BOOST_CHECK_EQUAL(taxi.GetPassenger(0).GetName(), "Person1");
			BOOST_CHECK_EQUAL(taxi.GetPassenger(2).GetName(), "PoliceMan1");

			BOOST_CHECK_THROW(taxi.GetPassenger(3).GetName(), out_of_range);
		}
		//можно высадить пассажира(пассажиров)
		BOOST_AUTO_TEST_CASE(can_drop_off_passenger_or_passengers)
		{
			CTaxi taxi(MakeOfTheCar::Volkswagen, 3);
			taxi.AddPassenger(person1);
			taxi.AddPassenger(racer1);
			taxi.AddPassenger(policeMan1);
			BOOST_CHECK(taxi.GetPassengerCount() == 3);

			taxi.RemovePassenger(0);
			BOOST_CHECK(taxi.GetPassengerCount() == 2);

			BOOST_CHECK_THROW(taxi.RemovePassenger(2), out_of_range);
			BOOST_CHECK(taxi.GetPassengerCount() == 2);

			taxi.RemoveAllPassengers();
			BOOST_CHECK(taxi.GetPassengerCount() == 0);
			BOOST_CHECK(taxi.IsEmpty());
		}
	BOOST_AUTO_TEST_SUITE_END()
	//Гоночный автомобиль
	BOOST_AUTO_TEST_SUITE(RacingCar)
		//изначально пуст
		BOOST_AUTO_TEST_CASE(initially_empty)
		{
			CRacingCar racingCar(MakeOfTheCar::Subaru, 2);
			BOOST_CHECK(racingCar.GetMakeOfTheCar() == MakeOfTheCar::Subaru);
			BOOST_CHECK(racingCar.GetPlaceCount() == 2);

			BOOST_CHECK(racingCar.IsEmpty());
			BOOST_CHECK(!racingCar.IsFull());
			BOOST_CHECK(racingCar.GetPassengerCount() == 0);
		}
		//можно посадить пассажира
		BOOST_AUTO_TEST_CASE(can_put_passenger)
		{
			CRacingCar racingCar(MakeOfTheCar::Subaru, 2);
			racingCar.AddPassenger(racer1);
			BOOST_CHECK(!racingCar.IsFull());
			BOOST_CHECK(racingCar.GetPassengerCount() == 1);

			racingCar.AddPassenger(racer2);
			BOOST_CHECK(racingCar.IsFull());
			BOOST_CHECK(racingCar.GetPassengerCount() == 2);

			BOOST_CHECK_THROW(racingCar.AddPassenger(racer3), logic_error);
			BOOST_CHECK(racingCar.GetPassengerCount() == 2);
		}
		//можно получить ссылку на пассажира
		BOOST_AUTO_TEST_CASE(can_get_reference_to_passenger)
		{
			CRacingCar racingCar(MakeOfTheCar::Subaru, 2);
			racingCar.AddPassenger(racer1);
			racingCar.AddPassenger(racer2);

			BOOST_CHECK_EQUAL(racingCar.GetPassenger(0).GetName(), "Racer1");
			BOOST_CHECK_EQUAL(racingCar.GetPassenger(1).GetName(), "Racer2");

			BOOST_CHECK_THROW(racingCar.GetPassenger(2).GetName(), out_of_range);
		}
		//можно высадить пассажира(пассажиров)
		BOOST_AUTO_TEST_CASE(can_drop_off_passenger_or_passengers)
		{
			CRacingCar racingCar(MakeOfTheCar::Subaru, 2);
			racingCar.AddPassenger(racer1);
			racingCar.AddPassenger(racer2);
			BOOST_CHECK(racingCar.GetPassengerCount() == 2);

			racingCar.RemovePassenger(0);
			BOOST_CHECK(racingCar.GetPassengerCount() == 1);

			BOOST_CHECK_THROW(racingCar.RemovePassenger(1), out_of_range);
			BOOST_CHECK(racingCar.GetPassengerCount() == 1);

			racingCar.RemoveAllPassengers();
			BOOST_CHECK(racingCar.GetPassengerCount() == 0);
			BOOST_CHECK(racingCar.IsEmpty());
		}
	BOOST_AUTO_TEST_SUITE_END()
	//Полицейский автомобиль
	BOOST_AUTO_TEST_SUITE(PoliceCar)
		//изначально пуст
		BOOST_AUTO_TEST_CASE(initially_empty)
		{
			CPoliceCar policeCar(MakeOfTheCar::Opel, 2);
			BOOST_CHECK(policeCar.GetMakeOfTheCar() == MakeOfTheCar::Opel);
			BOOST_CHECK(policeCar.GetPlaceCount() == 2);

			BOOST_CHECK(policeCar.IsEmpty());
			BOOST_CHECK(!policeCar.IsFull());
			BOOST_CHECK(policeCar.GetPassengerCount() == 0);
		}
		//можно посадить пассажира
		BOOST_AUTO_TEST_CASE(can_put_passenger)
		{
			CPoliceCar policeCar(MakeOfTheCar::Opel, 2);
			policeCar.AddPassenger(policeMan1);
			BOOST_CHECK(!policeCar.IsFull());
			BOOST_CHECK(policeCar.GetPassengerCount() == 1);

			policeCar.AddPassenger(policeMan2);
			BOOST_CHECK(policeCar.IsFull());
			BOOST_CHECK(policeCar.GetPassengerCount() == 2);

			BOOST_CHECK_THROW(policeCar.AddPassenger(policeMan3), logic_error);
			BOOST_CHECK(policeCar.GetPassengerCount() == 2);
		}
		//можно получить ссылку на пассажира
		BOOST_AUTO_TEST_CASE(can_get_reference_to_passenger)
		{
			CPoliceCar policeCar(MakeOfTheCar::Opel, 2);
			policeCar.AddPassenger(policeMan1);
			policeCar.AddPassenger(policeMan2);

			BOOST_CHECK_EQUAL(policeCar.GetPassenger(0).GetName(), "PoliceMan1");
			BOOST_CHECK_EQUAL(policeCar.GetPassenger(1).GetName(), "PoliceMan2");

			BOOST_CHECK_THROW(policeCar.GetPassenger(2).GetName(), out_of_range);
		}
		//можно высадить пассажира(пассажиров)
		BOOST_AUTO_TEST_CASE(can_drop_off_passenger_or_passengers)
		{
			CPoliceCar policeCar(MakeOfTheCar::Opel, 2);
			policeCar.AddPassenger(policeMan1);
			policeCar.AddPassenger(policeMan2);
			BOOST_CHECK(policeCar.GetPassengerCount() == 2);

			policeCar.RemovePassenger(0);
			BOOST_CHECK(policeCar.GetPassengerCount() == 1);

			BOOST_CHECK_THROW(policeCar.RemovePassenger(1), out_of_range);
			BOOST_CHECK(policeCar.GetPassengerCount() == 1);

			policeCar.RemoveAllPassengers();
			BOOST_CHECK(policeCar.GetPassengerCount() == 0);
			BOOST_CHECK(policeCar.IsEmpty());
		}
	BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
