#include "guest.h"
#include "list.h"
#include<iostream>
#include <cstdlib>

//Guest constructor
guest::guest() {

}

//return the ID of the guest
int guest::getId()
{
	return id;
}

//set the ID of the employee
void guest::setId(int Id)
{
	id = Id;
}


//get the assigned rooms of the guest
List<int> *guest::getRooms()
{
	return assignedRooms;
}

//set checkIn date for the guest
void guest::setCheckInDate(int cInDate)
{
	checkInDate = rand() % (11 - cInDate) + cInDate;
}

//get the checkIn date of the guest
int guest::getCheckInDate()
{
	return checkInDate;
}

//set checkOut date of the guest
void guest::setCheckOutDate(int cOutDate)
{
	checkOutDate = cOutDate;
}

//get checkOut date of the guest
int guest::getCheckOutDate()
{
	return checkOutDate;
}

//get number of rooms
int guest::getNumberRooms()
{
	return rooms;
}

//set number of nights
void guest::setNights()
{
	NumNights = (rand() % 4) + 1;
}

//get number of nights
int guest::getNumNights()
{
	return NumNights;
}

//set number of rooms
void guest::setNumberRooms()
{
	rooms = (rand() % 5) + 1;
}