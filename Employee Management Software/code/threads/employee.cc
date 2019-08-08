#include "string.h"
#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "list.h"
#include "employee.h"
#include<iostream>

//Employee constructor that calculates number of hours worked by an employee 
Employee::Employee() {
hoursWorked = (rand() % 21 + 20);
}

//return the ID of the employee
int Employee::getId()
{
	return id;
}

//set the ID of the employee
void Employee::setId(int Id)
{
	id = Id;
}

//return the name of the employee
string Employee::getName()
{
	return Name;
}

//set the name of the employee
void Employee::setName(string n)
{
	Name = n;
}

//return the department of the employee
int Employee::getDept()
{
	return Department;
}

//set the department of the employee
void Employee::setDept(int dpt)
{
	Department = dpt;
}

//return the position of the employee
int Employee::getpos()
{
	return position;
}

//set the position of the employee
void Employee::setpos(int pos)
{
	position = pos;
}

//return the hourly rate of the employee
int Employee::gethrate()
{
	return hourlyRate;
}

//set the hourly rate of the employee
void Employee::sethrate(int hr)
{
	hourlyRate = hr;
}

//return the number of hours worked by the employee
int Employee::getHoursWorked()
{
	return hoursWorked;
}

//return the paycheck of the individual employee
int Employee::getpayCheck()
{
	
	return (hourlyRate * hoursWorked);
}